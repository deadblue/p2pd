# Websocket API

## Message Specification

Message is the data transferred between client and server, there are three kinds of message:

* Request: Send from client to server, to ask server perform an action.
* Response: Send from server to client, to return result of an action which client requested before.
* Event: Send from server to client, to inform an event happened.

All of them are in JSON format.

### Request Structure

| Field  | Data Type | Comment            |
|--------|-----------|--------------------|
| id     | string    | Unique request ID. |
| method | string    | Method name.       |
| params | object    | Method parameters. |

Example:

```json
{
    "id": "unique-request-id",
    "method": "do_something",
    "params": {
        "foo": 1,
        "bar": "blabla"
    }
}
```

### Response Structure

| Field  | Data Type | Comment                        |
|--------|-----------|--------------------------------|
| id     | string    | ID the of related request.     |
| error  | integer   | Error code, `0` for succeeded. |
| result | object    | Action result, will be null when "error" is non-zero. |

Example:

```json
{
    "id": "related-request-id",
    "error": 0,
    "result": {
        "foo": 1,
        "bar": "blabla"
    }
}
```

### Event Structure

| Field | Data Type | Comment          |
|-------|-----------|------------------|
| id    | string    | Unique event ID. |
| name  | string    | Event name.      |
| data  | object    | Event data.      |

Example:

```json
{
    "id": "unique-event-id",
    "name": "something_happened",
    "data": {
        "foo": 1,
        "bar": "blabla"
    }
}
```

## Method Specification

### Add Task

**method: `task.add`**

**params:**

| Field | Data Type | Comment                           |
|-------|-----------|-----------------------------------|
| type  | string    | Task type: `magnet` or `torrent`. |
| data  | string    | A URI for `magnet` task, or base64 encoded torrent data for `torrent task`. |

**result:**

| Field   | Data Type | Comment                          |
|---------|-----------|----------------------------------|
| task_id | string    | Unique task ID.                  |
| name    | string    | Task name.                       |
| size    | integer   | Total size of all files in task. |

## Event

### Engine Alert

**name: `engine.alert`**

**data:**

| Field   | Data Type | Comment        |
|---------|-----------|----------------|
| message | string    | Alert message. |

### Task Finished

**name: `task.finished`**

**data:**

| Field   | Data Type | Comment         |
|---------|-----------|-----------------|
| task_id | integer   | Unique task ID. |
