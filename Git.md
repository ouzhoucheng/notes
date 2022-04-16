# Git使用

[为什么使用 Git](https://csdiy.wiki/%E5%BF%85%E5%AD%A6%E5%B7%A5%E5%85%B7/Git/)
[Git tutoria](https://missing.csail.mit.edu/2020/version-control/)
[Git 说明书](https://git-scm.com/book/zh/v2)

## 常用命令

### 基本命令

```C
git config --global user.name "John Doe" // 配置用户名及邮件地址，一次即可，对特定项目使用不同地址，去掉--global
git config --global user.email johndoe@example.com

git config --global core.editor emacs // 更换文本编辑器（如emacs

git config --list --show-origin // 查看配置及所在文件
git config --list // 检查配置
git config user.name // 检查一项配置（如user.name
git help config // 获取命令手册（如config
```

### 在已存在目录中初始化仓库

```C
cd // 工作文件夹
git init // 初始化，创建一个.git子目录
git add *.c // 追踪指定文件
git add -A // 追踪所有文件
git commit -m 'initial project version' // 无-m进编辑器写说明 -a 跳过暂存直接提交
```

### 克隆现存仓库

```python
git clone https://github.com/?/! # 克隆到一个文件夹，名为！
git clone https://github.com/?/! name # 新的目录名

```

### 常用命令

```C
git status // 查看文件状态 -s 紧凑输出
git diff // 显示变化
echo 'My Project' > README
rm A.md // 移除文件
git mv old new // 改名
```

[忽略一些文件](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E8%AE%B0%E5%BD%95%E6%AF%8F%E6%AC%A1%E6%9B%B4%E6%96%B0%E5%88%B0%E4%BB%93%E5%BA%93)
