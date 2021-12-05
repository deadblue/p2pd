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
| id     | `string`  | Unique request ID. |
| method | `string`  | Method name.       |
| params | `object`  | Method parameters. |

<details><summary>Example</summary>

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

</details>

### Response Structure

| Field  | Data Type | Comment                        |
|--------|-----------|--------------------------------|
| id     | `string`  | ID the of related request.     |
| error  | `integer` | Error code, `0` for succeeded. |
| result | `object`  | Action result, will be null when "error" is non-zero. |

<details><summary>Example</summary>

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

</details>

### Event Structure

| Field | Data Type | Comment     |
|-------|-----------|-------------|
| name  | `string`  | Event name. |
| data  | `object`  | Event data. |

<details><summary>Example</summary>

```json
{
    "name": "something_happened",
    "data": {
        "foo": 1,
        "bar": "blabla"
    }
}
```

</details>

## Method Specification

### Add Task

**description:** Create a task asynchronously, when task created, an event will be dispatched, 
so all clients will be informed at the same time.

**method: `task.add`**

**params:**

| Field | Data Type | Comment                           |
|-------|-----------|-----------------------------------|
| type  | `string`  | Task type: `magnet` or `torrent`. |
| uri   | `string`  | A URI for `magnet` task, or base64 encoded torrent data for `torrent` task. |

**result: `null`**

### List Task

**description:** Get task list with summary data.

**method: `task.list`**

**params: `null`**

**result: `array<TaskSummary>`**

`TaskSummary` structure:

| Field           | Data Type | Comment                          |
|-----------------|-----------|----------------------------------|
| id              | `string`  | Unique task ID.                  |
| name            | `string`  | Task name.                       |
| state           | `integer` | Task state.                      |
| total_bytes     | `integer` | Total bytes to download.         |
| dl_bytes        | `integer` | The bytes already downloaded.    |
| dl_speed        | `integer` | Current download speed in bytes. |
| ul_bytes        | `integer` | Total uploaded bytes.            |
| ul_speed        | `integer` | Current upload speed in bytes.   |
| connected_peers | `integer` | The number of connected peers.   |
| connected_seeds | `integer` | The number of connected seeds.   |

### Get Task Status

**description**: Get detail status of a task.

**method: `task.getStatus`**

**params:**

| Field   | Data Type | Comment  |
|---------|-----------|----------|
| task_id | `string`  | Task ID. |

**result:**

| Field   | Data Type        | Comment                          |
|---------|------------------|----------------------------------|
| pieces  | `array<bool>`    | Unique task ID.                  |
| files   | `array<integer>` | Task name.                       |
| peers   | `array<Peer>`    | Total size of all files in task. |

`Peer` structure:

| Field    | Data Type | Comment                                   |
|----------|-----------|-------------------------------------------|
| client   | `string`  | Client name.                              |
| address  | `string`  | Client IP address.                        |
| dl_bytes | `integer` | Total bytes we downloaded from this peer. |
| dl_speed | `integer` | Download speed from this peer.            |
| ul_bytes | `integer` | Total bytes we uploaded to this peer.     |
| ul_speed | `integer` | Upload speed to this peer.                |

### Get Task Metadata

**description**: Get Metadata of a task.

**method: `task.getMetadata`**

**params:**

| Field   | Data Type | Comment  |
|---------|-----------|----------|
| task_id | `string`  | Task ID. |

**result:**

| Field   | Data Type     | Comment                |
|---------|---------------|------------------------|
| id      | `string`      | Task ID.               |
| name    | `string`      | Torrent name.          |
| creator | `string`      | Torrent creator.       |
| comment | `string`      | Torrent comment.       |
| files   | `array<File>` | File list in torrent.  |
| ready   | `boolean`     | Is torrent data ready. |

`File` structure:

| Field   | Data Type     | Comment               |
|---------|---------------|-----------------------|
| path    | `string`      | File path in torrent. |
| size    | `integer`     | File size in bytes.   |

## Event

### Task Created

**description:** Fired when a task is created by a client.

**name: `task.created`**

**data:**

| Field   | Data Type     | Comment                |
|---------|---------------|------------------------|
| id      | `string`      | Task ID.               |
| name    | `string`      | Torrent name.          |
| creator | `string`      | Torrent creator.       |
| comment | `string`      | Torrent comment.       |
| files   | `array<File>` | File list in torrent.  |
| ready   | `boolean`     | Is torrent data ready. |

`File` structure:

| Field   | Data Type     | Comment               |
|---------|---------------|-----------------------|
| path    | `string`      | File path in torrent. |
| size    | `integer`     | File size in bytes.   |

### Task Metadata Received

**description:** Fired when a task received metadata from trackers or DHT nodes.

**name: `task.metadataReceived`**

**data:** Same as `task.created`.

### Task State Changed

**description:** Fired when a task received metadata from trackers or DHT nodes.

**name: `task.stateUpdated`**

**data:**

| Field     | Data Type | Comment                |
|-----------|-----------|------------------------|
| task_id   | `string`  | Task ID.               |
| old_state | `integer` | Torrent name.          |
| new_state | `integer` | Torrent creator.       |
