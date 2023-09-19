### error-message-error0308010cdigital-envelope-routinesunsupported
1. 降级到 Node.js v16
   
    mac:
```
    查看已安装的 Node.js 版本：
    brew list node
    这将显示您当前系统上已安装的 Node.js 版本列表。

    安装特定版本的 Node.js：
    brew install node@<version>

    将 `<version>` 替换为您希望安装的具体 Node.js 版本号，例如 `14`、`12` 等。运行此命令将安装指定版本的 Node.js。

    切换 Node.js 版本：
    brew unlink node
    brew link --overwrite --force node@<version>
    
    demo:
      brew list node
      brew install node@16
      brew unlink node
      brew link --overwrite --force node@16
      node -v
    
    stackoverflow问题地址：  
    https://stackoverflow.com/questions/69692842/error-message-error0308010cdigital-envelope-routinesunsupported  
```
这将取消当前 Node.js 版本的链接，并将新安装的版本链接为默认版本。

验证 Node.js 版本：
shell
Copy
node --version

2. 启用旧版 OpenSSL 提供程序。

    在类 Unix 上（Linux、macOS、Git bash 等）：

    export NODE_OPTIONS=--openssl-legacy-provider

    在 Windows 命令提示符下：

    set NODE_OPTIONS=--openssl-legacy-provider

    在 PowerShell 上：

    $env:NODE_OPTIONS = "--openssl-legacy-provider"
