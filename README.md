# investigator

Log analysis tool with support for bookmarks and multiple buffers. Investigator is aimed for efficient keyboard based interface. Additional features like fast file loading, efficient memory usage, and last workspace re-creation. 

Release plan
---------------

### Basic functionality
- fast file loading (for big files, mmap for posix)
- error and warnings defined in config file/Users/kamil/projects/investigator/src/ArgParser.hpp

### Additional functionality
- re-creating workspace after opening the file
- grepping based on the grep tool

### 1.2: Additional functionality
- readline support for the history

### Support for plugins
- attach to all controllers
- attach to all lists with callbacks
- access to instance objects for all interfaces

### 2.5: Support for Windows
- fast file loading (CreateFile/CreateFileMapping/MapViewOfFile)
- installer with desktop integration

### 3.0: GTK support
to be planned
