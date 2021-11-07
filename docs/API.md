# Websocket API

## Message Specification

Message is the data transferred between client and server, there are three kinds of it:

* Request: Send from client to server, to ask server perform an action.
* Response: Send from server to client, to return result of an action which client requested before.
* Event: Send from server to client, to inform an event happened.

All of them are in JSON format, 

### Request Structure

| Field  | Data Type | Comment |
|--------|-----------|---------|
| id     | string    | Unique request ID. |
| method | string    | Action name. |
| params | object    | Action parameters. |

### Response Structure

| Field  | Data Type | Comment |
|--------|-----------|---------|
| id     | string    | ID the of related request. |
| error  | integer   | Error code, `0` for succeeded. |
| result | object    | Action result, will be null when `error` is non-zero. |

### Event Structure

| Field | Data Type | Comment |
|-------|-----------|---------|
| id    | string    | Notification ID. |
| name  | integer   | Notification name. |
| data  | object    | Notification data. |

## Action List

TODO.