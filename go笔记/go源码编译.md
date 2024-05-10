git clone https://github.com/golang/go.git

#进入到src 目录执行 ./all.bash
cd src 
./all/bash

#编译好的 go 和 gofmt 在 bin 目录下：
cd ../bin/

# 为了防止我们修改的 go 和过去安装的 go 冲突，建立软连接
ln -sf /data/learn/go/bin/go mygo
# 加到~/.bash_profile
export PATH=/Users/bfr/Sites/go/go/bin:$PATH
source ~/.bash_profile

GODEBUG
我们在修改源码的时候，可以借助 GODEBUG 变量来打印调试信息。

schedtrace=X 表示运行时每 X 毫秒打印一行调度器的摘要信息到标准 err 输出中。
如设置 schedtrace=1000 程序开始后每个一秒就会打印一行调度器的概要信息 ：
[root@localhost gotest]# GOMAXPROCS=1 GODEBUG=schedtrace=1000 mygo run main.go SCHED 0ms: gomaxprocs=1 idleprocs=0 threads=4 spinningthreads=0 idlethreads=0 runqueue=0 [2]
# command-line-arguments
SCHED 0ms: gomaxprocs=1 idleprocs=0 threads=3 spinningthreads=0 idlethreads=1 runqueue=0 [2]
SCHED 0ms: gomaxprocs=1 idleprocs=0 threads=3 spinningthreads=0 idlethreads=0 runqueue=0 [2]

0ms ：自从程序开始的毫秒数；
gomaxprocs=1：配置的处理器数；
idleprocs=0：空闲的 P（处理器）数；
threads=3： 运行期管理的线程数，目前6个线程；
spinningthreads=0：执行抢占的线程数；
idlethreads=1：空闲的线程数；
runqueue=0： 在全局的run队列中的 goroutine 数；
[2]： 本地run队列中的goroutine数，表示有两个在等待；

schedtrace 和 scheddetail 一起设置可以提供处理器P,线程M和goroutine G的细节。

[root@localhost gotest]# GOMAXPROCS=1 GODEBUG=schedtrace=1000,scheddetail=1 mygo run main.go
SCHED 0ms: gomaxprocs=1 idleprocs=0 threads=4 spinningthreads=0 idlethreads=0 runqueue=0 gcwaiting=0 nmidlelocked=0 stopwait=0 sysmonwait=0
P0: status=0 schedtick=0 syscalltick=0 m=-1 runqsize=2 gfreecnt=0 timerslen=0
M3: p=-1 curg=-1 mallocing=0 throwing=0 preemptoff= locks=0 dying=0 spinning=false blocked=false lockedg=-1
M2: p=-1 curg=-1 mallocing=0 throwing=0 preemptoff= locks=2 dying=0 spinning=false blocked=false lockedg=-1
M1: p=-1 curg=17 mallocing=0 throwing=0 preemptoff= locks=0 dying=0 spinning=false blocked=false lockedg=17
M0: p=-1 curg=-1 mallocing=0 throwing=0 preemptoff= locks=1 dying=0 spinning=false blocked=false lockedg=1
G1: status=1(chan receive) m=-1 lockedm=0
G17: status=6() m=1 lockedm=1
G2: status=1() m=-1 lockedm=-1
G3: status=1() m=-1 lockedm=-1
G4: status=4(GC scavenge wait) m=-1 lockedm=-1
...

下面我们先看看 G 代表的意思：
status：G 的运行状态；
m：隶属哪一个 M；
lockedm：是否有锁定 M；
G 的运行状态大概有这些含义：
const (
    //  刚刚被分配并且还没有被初始化
    _Gidle = iota // 0
    // 没有执行代码，没有栈的所有权，存储在运行队列中
    _Grunnable // 1
    // 可以执行代码，拥有栈的所有权，被赋予了内核线程 M 和处理器 P
    _Grunning // 2
    // 正在执行系统调用，拥有栈的所有权，没有执行用户代码，
    // 被赋予了内核线程 M 但是不在运行队列上
    _Gsyscall // 3
    // 由于运行时而被阻塞，没有执行用户代码并且不在运行队列上，
    // 但是可能存在于 Channel 的等待队列上
    _Gwaiting // 4  
    // 表示当前goroutine没有被使用，没有执行代码，可能有分配的栈
    _Gdead // 6  
    // 栈正在被拷贝，没有执行代码，不在运行队列上
    _Gcopystack // 8
    // 由于抢占而被阻塞，没有执行用户代码并且不在运行队列上，等待唤醒
    _Gpreempted // 9
    // GC 正在扫描栈空间，没有执行代码，可以与其他状态同时存在
    _Gscan          = 0x1000
    ...
)

M 代表的意思：

M0: p=-1 curg=-1 mallocing=0 throwing=0 preemptoff= locks=1 dying=0 spinning=false blocked=false lockedg=1

p：隶属哪一个 P；
curg：当前正在使用哪个 G；
mallocing：是否正在分配内存；
throwing：是否抛出异常；
preemptoff：不等于空字符串（""）的话，保持 curg 在这个 m 上运行；
runqsize：运行队列中的 G 数量；
spinning：是否在抢占 G；

P 代表的意思：
P0: status=0 schedtick=0 syscalltick=0 m=-1 runqsize=2 gfreecnt=0 timerslen=0
status：P 的运行状态。
schedtick：P 的调度次数。
syscalltick：P 的系统调用次数。
m：隶属哪一个 M。
runqsize：运行队列中的 G 数量。
gfreecnt：可用的G（状态为 Gdead）。

P 的 status 状态代表的含义：
const (
    // 表示P没有运行用户代码或者调度器
    _Pidle = iota
    // 被线程 M 持有，并且正在执行用户代码或者调度器
    _Prunning
    // 没有执行用户代码，当前线程陷入系统调用
    _Psyscall
    // 被线程 M 持有，当前处理器由于垃圾回收 STW 被停止
    _Pgcstop
    // 当前处理器已经不被使用
    _Pdead
)

修改编译
func makechan(t *chantype, size int) *hchan {
...  
if debug.schedtrace > 0 {
print("bearluo makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
}
...
return c
}

然后进入到 go 的 src 目录下重新编译：
[root@localhost src]# ./make.bash
Building Go cmd/dist using /usr/local/go. (go1.15.8 linux/amd64)
Building Go toolchain1 using /usr/local/go.
Building Go bootstrap cmd/go (go_bootstrap) using Go toolchain1.
Building Go toolchain2 using go_bootstrap and Go toolchain1.
Building Go toolchain3 using go_bootstrap and Go toolchain2.
Building packages and commands for linux/amd64.
Installed Go for linux/amd64 in /data/learn/go
Installed commands in /data/learn/go/bin

编写一个简单的demo：
package main

import (
"fmt"
)

func main() {

	c := make(chan int, 10)
	fmt.Println(c)
}
[root@localhost gotest]# GODEBUG=schedtrace=1000 mygo run main.go
bearluo makechan: chan=0xc000036070; elemsize=8; dataqsiz=2
SCHED 0ms: gomaxprocs=16 idleprocs=13 threads=6 spinningthreads=1 idlethreads=0 runqueue=0 [1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
bearluo makechan: chan=0xc00010e000; elemsize=1; dataqsiz=0
bearluo makechan: chan=0xc00010e060; elemsize=0; dataqsiz=0
bearluo makechan: chan=0xc00010e180; elemsize=0; dataqsiz=0
bearluo makechan: chan=0xc0006a8000; elemsize=1; dataqsiz=35
bearluo makechan: chan=0xc0003f6660; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000226540; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc0001381e0; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc0005043c0; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc00049c420; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000594300; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000090360; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000220000; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc00075e000; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000138840; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc000226780; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc0003ea420; elemsize=16; dataqsiz=2
bearluo makechan: chan=0xc00049d320; elemsize=16; dataqsiz=1
...















