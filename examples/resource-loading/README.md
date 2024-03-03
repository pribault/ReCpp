This example demonstrates the use of ReCpp to download in parallel multiple json files, and how it can improve resource loading.

This example will run two tests, the first one will try to load resources in an old fashion way on a single thread, the second thread will launch each resource load on a thread pool. That's why the seond "test ended" log is printed on a different thread.

Running this example should output something as follows:
```
[thread 25608] main thread id: 25608
[thread 25608]
[thread 25608] starting 'standard' test
[thread 25608] test ended, took 68ms
[thread 25608]
[thread 25608] starting 'reactive' test
[thread 28852] test ended, took 10ms
```
