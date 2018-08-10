# 環境  

## Raspberry PI  

sudo apt-get install libczmq-dev  


## ホストPC  

python  
pip install matplotlib  
pip install numpy  
pip install zmq  

# 実行

## Raspberry PI  

```
$ make  
$ chmod a+x ./gyro_profiler  
$ sudo ./gyro_profiler  
```

'q'+'Enter' to stop profing.

## ホストPC   

```
python show_gyro_graph.py  
```

