# Monitoring

### Load dependencies
~~~
mkdir alice && cd alice
aliBuild init
aliBuild --default o2-daq build QualityControl
alienv enter QualityControl/latest-o2-daq
~~~

aliBuild help: http://alisw.github.io/alibuild/o2-daq-tutorial.html

### Compilation
~~~
git clone REPOS
cd TObejct2JSON
mkdir build && cd build
cmake ..
make -j
~~~

### Use
~~~
ssh -L 3306:127.0.0.1:3306 root@mysqlserverhost OR start sql locally
./build/bin/tobject2json
./bin/test_zmq
~~~


### Documentation
* ZeroMQ http://api.zeromq.org/4-2:_start
