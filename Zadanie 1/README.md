## Response convention

A response is a success if it lacks an `"error"` key.

```jsonc
{"reservationId": 1}

{"error": "sold_out"}
```

## Verbs

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

The client  sends this when it can't complete the sale locally -  user hits Cancel or drawer can't make change.

## Bad-request errors

Malformed input (invalid JSON, missing `verb`, unknown verb, missing or wrong-typed field) returns:

```jsonc
< {"error": "bad_request", "detail": "missing field 'reservationId'"}
```
