# Bookstore

`Author` : `jy`

## 目的

实现一个账户管理系统和书籍管理系统，使用块状链表实现数据存储。

## 交互方式

使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。

指令合法字符集为标准 ASCII 字符；允许的空白符仅为空格，单个指令被空格分割为多个部分。多个连续空格效果与一个空格等价；行首行末允许出现多余空格；如无特殊说明禁止省略或另增空格。

指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配。

本文档中以弱化的正则语法说明指令文法。合法指令的文法相关说明如下：

- `[x]` 表示一串有特定限制的用户自定义字符串；
- `(a|b|c)` 表示此处仅能出现 `a`, `b`, `c` 中其一；
- `(x)?` 表示此处可以出现零次或一次 `x`；
- `(x)+` 表示此处可以出现一次或多次 `x`。

合法的指令如下所示：（`#` 开头的行是注释）

```
# 基础指令
quit
exit

# 帐户系统指令
su [UserID] ([Password])?
logout
register [UserID] [Password] [Username]
passwd [UserID] ([CurrentPassword])? [NewPassword]
useradd [UserID] [Password] [Privilege] [Username]
delete [UserID]

# 图书系统指令
show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
buy [ISBN] [Quantity]
select [ISBN]
modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
import [Quantity] [TotalCost]

# 日志系统指令
show finance ([Count])?
log
```


## 帐户系统

书店管理系统服务于 **店主（超级管理员）**，**员工（管理员）以及顾客（用户）**。为满足其不同需求，故需帐户系统管理帐户及权限数据，并提供相应功能。

### 权限

本文档中以 {x} 表示权限等级，数字越大则权限越高。帐户的权限等级有 {7}, {3} 和 {1}；未登录任何帐户的状态下当前帐户权限视为 {0}。低权限帐户能够访问的功能严格是高权限帐户能够访问的功能的子集，指令的权限等级表示能够执行该指令的最低当前帐户权限需求。换句话说，高权限的帐户可以执行需要低权限的指令，反之则不可。


### 各权限可访问功能

- **店主 {7}**：使用 `root` 帐户，可以访问书店系统所有功能，包括日志和帐户管理功能；
- **员工 {3}**：可以访问修改图书数据相关功能等；
- **顾客 {1}**：可以注册帐户，查询、购买图书；
- **游客 {0}**：可以注册帐户。

本系统中有且仅有以上四种权限等级，其余权限等级非法。

### 登录栈

系统支持嵌套登录，即允许多个帐户同时处于登录状态；允许同一帐户同时多次登录。输入的指令视为最后登录的帐户操作。退出系统视为登出所有已登录帐户。

示例：（假设所有用户均存在，所有密码均正确）

```bash
# 初始时登录栈为空
su root sjtu
select first
# 此时登录栈为 root，选择的书为 first

su root sjtu
# 此时登录栈为 root root，没有选择的书
select second

su zhang beihai
select third
# 此时登录栈为 root root zhang，选择的书为 third
# 操作的用户为 zhang，无法执行 delete 等命令

su chu yan
# 此时登录栈为 root root zhang chu，操作的用户为 chu，没有选择的书

logout
# 此时登录栈为 root root zhang，选择的书为 third

logout
# 此时登录栈为 root root，选择的书为 second

logout
# 此时登录栈为 root，选择的书为 first
```

### 帐户信息

关于指令格式中的用户自定义字符串限定如下，其中「最大长度」含义为该字符串所占存储空间不能超过指定数目的英文字母字符所占存储空间。

- `[UserID]`, `[Password]`, `[CurrentPassword]`, `[NewPassword]`
  - 合法字符集：数字，字母，下划线；
  - 最大长度：30。
- `[Username]`
  - 合法字符集：除不可见字符以外 ASCII 字符；
  - 最大长度：30。
- `[Privilege]`
  - 合法字符集：数字；
  - 最大长度：1。


出现不符合上述限定的自定义字符串的指令视为非法，下同。

### 帐户相关指令

本文档中指令说明格式如下：

- **指令名称**
  - {指令权限}  `[指令格式]`
  - 指令功能
  - 指令逻辑（条数不定，各逻辑间优先顺序同列举顺序）

以下为指令说明。

- **登录帐户**
  - {0} `su [UserID] ([Password])?`
  - 使登录帐户变为已登录状态，当前帐户变为该帐户。
    - 如果该帐户不存在则操作失败；
    - 如果密码错误则操作失败；
    - 如果当前帐户权限等级高于登录帐户则可以省略密码。
- **注销帐户**
  - {1} `logout`
  - 撤销最后一次成功执行的 `su` 指令效果。
    - 如无已登录帐户则操作失败。
- **注册帐户**
  - {0} `register [UserID] [Password] [Username]`
  - 注册信息如指令格式所示，权限等级为 {1} 的帐户。
    - 如果 `[UserID]` 与已注册帐户重复则操作失败。
- **修改密码**
  - {1} `passwd [UserID] ([CurrentPassword])? [NewPassword]`
  - 修改指定帐户的密码。
    - 如果该帐户不存在则操作失败；
    - 如果密码错误则操作失败；
    - 如果当前帐户权限等级为 {7} 则可以省略 `[CurrentPassword]`。
- **创建帐户**
  - {3} `useradd [UserID] [Password] [Privilege] [Username]`
  - 创建信息如指令格式所示的帐户。
    - 如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败；
    - 如果 `[UserID]` 与已注册帐户重复则操作失败。
- **删除帐户**
  - {7} `delete [UserID]`
  - 删除指定帐户。
    - 如果待删除帐户不存在则操作失败；
    - 如果待删除帐户已登录则操作失败。

## 图书系统

图书系统提供图书信息、库存信息和图书交易财务记录的相关服务及数据存储功能。

### 图书信息

- `[ISBN]`：Internal Strange Book Number；
  - 合法字符集：除不可见字符以外 ASCII 字符；
  - 最大长度：20；
  - **特殊说明：** 本系统中任何两本图书不应有相同 `[ISBN]` 信息。
- `[BookName]`, `[Author]`：图书名字，图书作者名字；
  - 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
  - 最大长度：60。
- `[Keyword]`：图书关键词；
  - 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
  - 最大长度：60；
  - **特殊说明**：`[keyword]` 内容以 `|` 为分隔可以出现多段信息。如 `math|magic|quantum` 表示该图书具有三个无顺序的关键词 `math`, `magic` 和 `quantum`，每段信息长度至少为 1。
- `[Quantity]`：购买数量；
  - 合法字符集：数字；
  - 最大长度：10；
  - 特殊说明：数值不超过 2'147'483'647。
- `[Price]`, `[TotalCost]`：图书单价，交易总额。
  - 合法字符集：数字和 `.`；
  - 最大长度：13；
  - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位。

字符相关信息默认值为空，数字相关信息默认值为数值 0。

### 图书相关指令

- **检索图书**
  - {1} `show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?`
  - 以 `[ISBN]` 字典升序依次输出满足要求的图书信息，每个图书信息输出格式为 `[ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n`，其中 `[Keyword]` 中关键词顺序为输入时的顺序。
    - 无满足要求的图书时输出空行；
    - 无附加参数时，所有图书均满足要求；
    - 附加参数内容为空则操作失败；
    - `[Keyword]` 中出现多个关键词则操作失败。

- **购买图书**
  - {1} `buy [ISBN] [Quantity]`
  - 购买指定数量的指定图书（由于本系统不包括支付系统，故仅需减少库存），以浮点数输出购买图书所需的总金额。
    - 没有符合条件的图书则操作失败；
    - 购买数量为非正整数则操作失败。

- **选择图书**
  - {3} `select [ISBN]`
  - 以**当前帐户**选中指定图书（帐户登出后无需保存选中图书情况）。
    - 没有符合条件的图书则创建仅拥有 `[ISBN]` 信息的新图书；
    - 退出系统视为取消选中图书。

- **修改图书信息**
  - {3} `modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+`
  - 以指令中的信息更新选中图书的信息。
    - 如未选中图书则操作失败；
    - 有重复附加参数为非法指令；
    - 附加参数内容为空则操作失败；
    - 不允许将 ISBN 改为原有的 ISBN，例如：
      ```
      select example
      modify -ISBN=example
      ```
      为非法操作；
    - `[keyword]` 包含重复信息段则操作失败。

- **图书进货**
  - {3} `import [Quantity] [TotalCost]`
  - 以指定交易总额购入指定数量的选中图书，增加其库存数。
    - 如未选中图书则操作失败；
    - 购入数量为非正整数则操作失败；
    - 交易总额为非正数则操作失败。

## 文件和代码结构

```markdown
main.cpp
├── interpreter.h
│ ├── tokenScanner.hpp
│ │ └── TokenScanner 
│ ├── user.h
│ │ ├── book
│ │ └── user
│ └── writer.cpp//块状连模板类
└── error.hpp
```

```markdown
main.cpp
└── Interpreter interpreter
 ├── int n
 ├── int priority
 ├── std::vector<std::string>ps
 ├── std::stack<user>users
 ├── std::vector<std::string>loggerUsers
 ├── writer<book>book_manager
  ├── w(T&)
  ├── d(T&)
  └── f(T&, std::set<T>&)
 ├── writer<user>user_manager
 ├── std::vector<std::pair<double, double>>finance
 ├── std::vector<std::string>cmds
 │
 ├── void init()
 ├── void input(const std::string &s)
 ├── void run()
 ├── bool p()//判定是否该终止程序
 └── ~Interpreter()
```

## 文件存储说明

块状链表存储