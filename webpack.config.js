// 引入一个包
const path = require('path')

// webpack中所有的配置信息都应该写在module.exports中
module.exports = {
    // 指定入口文件
    entry: "./src/index.ts",
    // 指定打包文件所在目录
    output: {
        // 指定打包文件目录
        path: path.resolve(__dirname, 'dist'),
        // 打包后的文件名
        filename: "bundle.js"
    },
    // 指定webpack打包时要使用模块
    module: {
        // 指定图片、资源等要加载的规则
        rules: [
            {
                // test指定的是规则生效的文件
                test: /\.ts$/,//匹配所有以ts结尾的文件
                use: 'ts-loader',//用ts-loader去处理以ts结尾的文件
                // 要排除的文件
                exclude: /node-modules/
            }
        ]
    }
}