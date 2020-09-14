# Building project
Create  **build** directory in main folder and run following command from it:
```
cmake ../ -DCMAKE_PREFIX_PATH:STRING=<path to Qt files> -DSANITIZE=<ON/OFF> && make
```
Example:
```
cmake ../ -DCMAKE_PREFIX_PATH:STRING=/opt/Qt5.11.1/5.11.1/gcc_64 -DSANITIZE=ON && make
```

# Running
In main project binary folder run: 
```
./bin/TestHost
```

