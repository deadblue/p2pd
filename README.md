# Make P2P Great Again!

```
            ___             __
      ____ |__ \ ____  ____/ /
     / __ \__/ // __ \/ __  / 
    / /_/ / __// /_/ / /_/ /  
   / .___/____/ .___/\__,_/   
  /_/        /_/              

```

A hobby project for C++ exercise.

## Feature

Run BT downloading in daemon, with management API over websocket.

* Telnet interface is in plan.
* ED2k support is in plan.

## Build

### Dependencies

* boost >= 1.66.0 (Tested on 1.69.0 & 1.76.0)
* libtorrent-rasterbar >= 1.2.x (Tested on 1.2.14 & 2.0.4)
* json-c >= 0.13 (Tested on 0.13 and 0.15)

Please install (or build) them on your build environment.

### Compile

```bash
mkdir -p /dir/for/building
cd /dir/for/building
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" /path/to/this_project
make 
make install
```

## Usage 

```bash
./p2pd
```

## License

MIT
