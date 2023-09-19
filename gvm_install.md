# mac install gvm
### 1、xcode-select --install
### 2、brew update
### 3、brew install mercurial
### 4、brew install bison
### 5、bash < <(curl -s -S -L https://raw.githubusercontent.com/moovweb/gvm/master/binscripts/gvm-installer)
### 6、source /Users/bfr/.gvm/scripts/gvm

#### 常用命令：
    gvm listall // go版本列表
    gvm install go1.20.3 // 下载指定版本的go
    gvm use go1.20.3 --default  // 切换默认go版本
    gvm implode // 卸载已安装的go版本


SELECT SUM(usage) / (COUNT(cpu) * 100) AS used
FROM (
SELECT time, cpu, host, 100 - usage_idle AS usage
FROM (
SELECT *,
ROW_NUMBER() OVER (PARTITION BY host, cpu ORDER BY time DESC) AS rn
FROM cnosdb_admin_stats.cpu
WHERE host IN ('localhost') AND cpu != 'cpu-total'
ORDER BY time DESC
) AS ranked_cpu
WHERE rn = 1
);device

SELECT SUM(usage) / (COUNT(device) * 100) AS used
FROM (
SELECT time, device, host, used_percent AS usage
FROM (SELECT *, ROW_NUMBER() OVER (PARTITION BY host,used_percent ORDER BY time DESC) AS rn 
FROM cnosdb_admin_stats.disk
WHERE host IN ('localhost')
ORDER BY time DESC
) AS ranked_cpu
WHERE rn = 1
);