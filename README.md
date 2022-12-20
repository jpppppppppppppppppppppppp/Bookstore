# Bookstore

三大基本模块：账户系统，图书系统，日志系统

账户系统应该用一个二进制文件进行存储便于修改，UserName的长度也应该保存下来

对指令写一个解释器封装成类，可以借鉴TokenScanner

用vector记录已经登陆的账户，方便delete的调用

新建类Interpreter，里面含有TokenScanner和登录栈和登陆用户数组

在Interpreter中解释语句，修改、读写文件

建立类User和Book，

User含有UserName，UserId，UserPW，type，select

Book含有ISBN，BookName，Auther，Keyword，Quantity，Price，TotalCost

块状链表实现

开头是文件包含元素个数——4bit

ID的最小值——统一长度

ID的最大值——统一长度

ID按顺序排序罗列内容

