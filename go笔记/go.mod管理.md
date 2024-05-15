#### go.mod文件追加：
replace github.com/xxxx/xxxx => xxx/xxx  这样可以指定本地的包
#### go vender 缓存到本地
##### go mod vendor             // 将mod文件缓存到本地
##### go build -mod vendor      // 编译文件，指定包为缓存包

#### 创建 Go Module
##### 删除本地go.mod
go mod init github.com/xxx/xxx
##### 推送至代码仓库
##### 增加新版本时，在仓库打新tag

###### Go Modules 将每个包视为一个git项目
###### gomod 就是解决"需要哪个git项目的什么版本"
###### 无法连接远程仓库时，使用重定向或者mod vender方案