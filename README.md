# Monitoring

### Installation (CC7)
~~~
aliBuild --defaults root6 build zeromq root
~~~

aliBuild installation procedure: http://alisw.github.io/alibuild/o2-daq-tutorial.html

### Configuration
~~~
alienv load ROOT/latest-root6
alienv enter ZeroMQ/latest-root6
~~~

### Compilation
~~~
cd TObejct2JSON
mkdir build; cd build
cmake ...
make -j
~~~

### Use
~~~
. /Applications/root_v6.10.08/bin/thisroot.sh
xcode-select --install
~~~


### Documentation
* http://api.zeromq.org/2-1:zmq-cpp
* https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-getting-started-examples.html (API ref does not exist)
* https://docs.oracle.com/javase/8/docs/api/index.html?java/sql/PreparedStatement.html (API for Java, same as C++)
