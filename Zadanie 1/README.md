# Ticketmat

A console ticket-vending system over TCP. The server holds a shared ticket pool with timed reservations. Clients act as kiosks: each owns its own cash drawer and walks the user through picking a ticket, entering a name, paying with coins, and getting change.

## Build

**Requirements:** CMake 3.15+, Conan 2.x, C++20 compiler

```bash
conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Run

Start the server:
```bash
./build/Release/ticketmat_server
```

Then a client (in another terminal):
```bash
./build/Release/ticketmat_client
```

The kiosk lists available tickets, lets you pick one, asks for a name, takes coins (e.g. `200 100 50` for 2.00, 1.00, 0.50 PLN), prints the change and a ticket number.

## Tests

```bash
./build/Release/cash_register_tests
./build/Release/ticket_store_tests
```

## Wire protocol

Newline-delimited JSON over TCP. Client sends one verb per line, server replies with one JSON object per line.

A response is a success if it lacks an `"error"` key.

```jsonc
{"reservationId": 1}

{"error": "sold_out"}
```

### `LIST_TICKETS`

```jsonc
> {"verb": "LIST_TICKETS"}
< {"tickets": [
    {"type": "normal", "price": 350, "available": 10},
    {"type": "ulgowy", "price": 170, "available": 5}
  ]}
```

Returns all ticket types, including the sold-out ones.

### `RESERVE`

```jsonc
> {"verb": "RESERVE", "type": "normal"}
< {"reservationId": 1}
< {"error": "sold_out"}
< {"error": "unknown_type"}
```

Reserves one ticket of the given type. The reservation expires 60s after creation.

### `FINALIZE`

```jsonc
> {"verb": "FINALIZE", "reservationId": 1, "name": "Jan Kowalski"}
< {"ticketNumber": 42}
< {"error": "expired"}
```

Commits a reservation into a sold ticket. The client calls this after collecting payment locally.

### `CANCEL`

```jsonc
> {"verb": "CANCEL", "reservationId": 1}
< {"cancelled": true}
```

Always returns `{"cancelled": true}`, even if the reservation is already gone.

### Bad-request errors

Malformed input (invalid JSON, missing `verb`, unknown verb, missing or wrong-typed field) returns:

```jsonc
< {"error": "bad_request", "detail": "missing field 'reservationId'"}
```
