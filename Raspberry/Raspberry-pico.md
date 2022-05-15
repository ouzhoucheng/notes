# 树莓派pico下载和停止运行main.py

## 用vscode调试pico
[pico+vscode](https://blog.csdn.net/qq_44796348/article/details/113574979)
- 在插件中搜索`RT-Thread MicroPython`并安装
- 插上pico
- 在状态栏选择连接串口,连接
- 在状态栏选新建工程,建一个空工程,添加`main.py`文件
- 添加代码
  ```python
    from machine import Pin
    import time
    LED = Pin(25,Pin.OUT)
    if __name__ == '__main__':
        while True:
            LED.value(0)
            time.sleep(1)
            LED.value(1)
            time.sleep(1)
  ```
- 点击运行,可看到led闪烁

## 下载工程
- ![在这里插入图片描述](https://img-blog.csdnimg.cn/4efe3cecf7574a21bf5ff934d5ceca3c.png)
- 对于名为`main.py`文件,pico上电会自动运行
- **注意**下载后如果main内有`while`循环,pico会一直执行,期间不能打断,调试和下载新工程

## 清除flash
- 如果要清掉flash,断电,先按住`loader`键,再重新上电,磁盘里会多一个pico的空间
- 将文件`flash_nuke.uf2`粘贴到里面,清除flash,再粘贴`rp2-pico-20220117-v1.18.uf2`固件,这时会自动复位,就可以再次调试和下载了
- 有亿点麻烦,所以没事别随便下载 :(
- [这两个文件可以在此处找到](https://gitee.com/Wind_to_valley/notes.git)