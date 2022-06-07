[TOC]

# 文件操作

```shell
cd # 进入一个目录
cd / # 根目录
cd ~ # 家目录
cd .. # 上一级目录
cd - # 返回刚才所在
pwd # 查看当前目录

# 文件
touch asd.txt fgh.txt
touch love_{1..10}_shiyanlou.txt # 一次多个文件
chmod 777 <文件> # 开启读写权限

# 文件夹
mkdir # -p 多级目录
tree # 查看文件树
ls # 查看当前文件夹

# 操作
cp test father/son/grandson # 复制去
cp /etc/passwd passwd # 复制来
cp -r father family # 复制目录

rm <file> # 删除文件 -f 强制删除 -r 删除目录
mv file1 Documents # 移动
rename # 重命名
cat # 打印文件内容
file # 文件类型

# 进程
sudo kill -9 507
```

安装

```shell
sudo apt-get upgrate # 更新不处理依赖
sudo apt-get update # 获取最近软件包列表
sudo apt-get install <包> # 安装某个包
```

压缩&解压

```shell
zip something.zip something # 打包 ： （目录请加 -r 参数）
zip -r -9 -q -o shiyanlou_9.zip /home/shiyanlou/Desktop -x ~/*.# du -h 查看打包后大小
# -r 递归打包子目录所有 
# -q 不打印信息 
# -o 文件名 输出文件 
# -9/-1 压缩级别 
# -x 排除上一次的zip文件 	
# -e 加密压缩包
# -l linux2windows

unzip something.zip # 解包 指定路径：-d 参数
# -q 安静模式
# -l 不解压只看内容
# -0 指定编码
unzip -O GBK 中文压缩文件.zip

tar -cf something.tar something # 打包
tar -xf something.tar # 解包：
-C 参数 指定路径
# -c 创建tar包
# -f 创建指定文件名
tar -P -cf shiyanlou.tar /home/shiyanlou/Desktop
# -x 解包
# -C 到已存在目录
mkdir tardir
tar -xf shiyanlou.tar -C tardir
# -t 只查看不解包
# -z gzip压缩
tar -czf shiyanlou.tar.gz /home/shiyanlou/Desktop
# -x 解压tar.gz		
tar -xzf shiyanlou.tar.gz 

```

# vmware
[VMware Tools 显示灰色](http://t.zoukankan.com/fanyf-p-11583329.html)
[手动安装VMwareTools](https://blog.csdn.net/sudaroot/article/details/119084203)

# 自定义分辨率

更改一个1920x515的分辨率, 按顺序输入下列指令

```shell
cvt 1920 515
sudo xrandr --newmode "1920x515_60.00"   77.50  1920 1976 2168 2416  515 518 528 536 -hsync +vsync
xrandr --addmode Virtual1 "1920x515_60.00"
sudo xrandr --output Virtual1 --mode "1920x515_60.00"
```

重启后还是变回原来的分辨率,按网上更改`./profile`的方法,重启黑屏,被迫重装. 更改写成脚本,每次开机打开即可,或者挂起不关机

```shell
touch Resolution.sh # 新建脚本文件
# 复制上一个框的内容,保存
chmod 777 Resolution.sh # 开启权限

# 每次开机在终端内启动一下.sh文件即可
```
