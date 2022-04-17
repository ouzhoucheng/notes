# Git使用

[为什么使用 Git](https://csdiy.wiki/%E5%BF%85%E5%AD%A6%E5%B7%A5%E5%85%B7/Git/)
[Git tutoria](https://missing.csail.mit.edu/2020/version-control/)
[Git 说明书](https://git-scm.com/book/zh/v2)

## 常用命令

### 一般步骤

```C
cd // 工作文件夹
git init // 初始化，创建一个.git子目录
git add *.c // 追踪指定文件
git add -A // 追踪所有文件
git commit -m 'initial project version' // 无-m进编辑器写说明 -a 跳过暂存直接提交 --amend 重新提交

git remote add <shortname> <url> // 添加一个新远程git仓库
git fetch <shortname> // 拉取远程仓库有本地无的信息

git push <remote> <branch> // 提交到远程仓库 -u -f 强行
```

### 分支与合并

```C
git branch // 查看当前分支 --no-merged 查看未合并工作的分支
git branch testing // 新testing分支
git checkout testing // 切换到新创建的分支 git checkout -b testing 简写
git merge testing // 将testing合并到当前分支
git add -- // 冲突已解决
git branch -d testing // 合并后删除该分支
```

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

### 克隆现存仓库

```python
git clone https://github.com/?/! # 克隆到一个文件夹，名为！
git clone https://github.com/?/! name # 新的目录名

```

### 其他常用命令

```C
git status // 查看文件状态 -s 紧凑输出
git diff // 显示变化
echo 'My Project' > README
rm A.md // 移除文件
git mv old new // 改名
git log // 时间先后列出提交 -p 每次提交的差异
git reset HEAD <file> // 取消暂存
git checkout -- <file> // 撤销修改，回到上一次提交状态

git remote // 查看远程仓库 -v 显示简写与url
git remote rename <OldName> <NewName> // 修改一个远程仓库简写
git remote remove paul // 移除一个远程仓库

git tag // 列出标签
git tag -l "v1.8.5*"// 列出标签
git tag -a v1.4 -m "my version 1.4" // 附注标签
git tag v1.4 // 轻量标签
// 后期打标签
git log --pretty=oneline // 列出所有提交校验和
git tag -a v1.2 9fceb02 // 对应部分校验和
git push origin v1.5 // 共享标签
git push origin --tags // 推送所有标签
git tag -d v1.4-lw // 删除一个标签
git tag origin --delete <tagname> // 删除远程标签

git rebase master // 变基
// https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E5%8F%98%E5%9F%BA

```

[忽略一些文件](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E8%AE%B0%E5%BD%95%E6%AF%8F%E6%AC%A1%E6%9B%B4%E6%96%B0%E5%88%B0%E4%BB%93%E5%BA%93)
