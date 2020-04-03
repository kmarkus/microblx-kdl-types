# microbx-kdl-types

This is a microblx type module for using basic types of the
[Kinematics and Dynamics Library
(KDL)](https://github.com/orocos/orocos_kinematics_dynamics) library.

## Installation

After installing `microblx` run

```sh
$ mkdir build
$ cd build
$ cmake ..
...
$ make && sudo make install
...
```

The resulting microblx module is called `kdl_types`

The type headers are install under `${prefix}/kdlubx/` and can thus be
included as follows:

```C
#include <kdlubx/kdl.h>
```

## License

microblx-kdl is licensed under
[BSD-3-Clause](https://spdx.org/licenses/BSD-3-Clause).
