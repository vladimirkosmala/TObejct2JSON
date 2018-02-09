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
. /Applications/root_v6.10.08/bin/thisroot.sh
xcode-select --install
ssh -L 3306:127.0.0.1:3306 root@mysqlserverhost OR start sql locally
./build/bin/tobject2json
./bin/test_zmq
~~~


### Documentation
* http://api.zeromq.org/2-1:zmq-cpp
* https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-getting-started-examples.html (API ref does not exist)
* https://docs.oracle.com/javase/8/docs/api/index.html?java/sql/PreparedStatement.html (API for Java, same as C++)
