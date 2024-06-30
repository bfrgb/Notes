<template>
  <div id="uploader">
    <input
      v-if="uploadPDF"
      :id="fileID"
      :ref="fileID"
      type="file"
      name="file"
      multiple="multiple"
      accept="image/*;application/*"
      style="display:none;"
      @change="UpladFile"
    />
    <input
      v-if="!uploadPDF"
      :id="fileID"
      :ref="fileID"
      type="file"
      name="file"
      multiple="multiple"
      accept="image/*"
      style="display:none;"
      @change="UpladFile"
    />
    <!--
    <input type="file" :id="fileID + '2'" multiple="multiple" name="file" :ref="fileID + '2'" accept="image/*" @change="UpladFile"/>
    <section :id="sectionID" class="widget-ui-pushbutton pushbutton-in" style="display:none;">
      <div class="widget-ui-pushbutton-list" style="max-height:240px;">
        <a href="javascript:void(0);" tabindex="0" class="list-a takePictures" data-key="takePictures" @click="UploadFileType(1)">拍照</a>
        <a href="javascript:void(0);" tabindex="1" name="file" class="list-a " key1="video1" key2="video2" @click="UploadFileType(2)">本地上传</a>
        <a href="javascript:void(0);" class="pushbutton-cancel" @click="upload">取消</a>
      </div>
    </section>
    -->

    <div v-show="!hidehtml" class="weui-cells vux-no-group-title">
      <div class="weui-cell">
        <div class="weui-cell__hd"></div>
        <div class="vux-cell-bd vux-cell-primary">
          <p>
            <label v-if="message" class="vux-label">{{ message }}</label>
          </p>
          <span class="vux-label-desc"></span>
        </div>
        <div class="weui-cell__ft">
          <div capture="camera" class="avatar-uploader">
            <div tabindex="0" class="el-upload el-upload--text">
              <i class="el-icon-picture avatar-uploader-icon"></i>
              <span style="font-size: 1.2rem;" @click="upload(field)"
                ><a>上传{{ message }}</a></span
              >
            </div>
          </div>
        </div>
      </div>
    </div>
    <loading :show="showLoading" text="上传中..."></loading>
  </div>
</template>
<script>
import { i18nHelper } from '@it-common/lbp_common'
import { Loading, TransferDom } from 'vux'
export default {
  name: 'uploader',
  components: {
    Loading,
  },
  directives: {
    TransferDom,
  },
  props: {
    message: { type: String, default: '' },
    field: { type: String, default: '' },
    hidehtml: { type: Boolean, default: false },
    uploadPDF: { type: Boolean, default: false },
  },
  data() {
    return {
      xhr: '',
      id: '',
      imgUrl: '',
      ot: '',
      loaded: '',
      isPhoto: '1',
      sectionID: '',
      fileID: '',
      startDate: '',
      endDate: '',
      showLoading: false,
    }
  },
  created() {
    this.id = this.$route.query.id
    this.vacation = this.$route.query.type_id ? this.$route.query.type_id : ''
    this.abnormal = this.$route.query.abnormal_id ? this.$route.query.abnormal_id : ''

    this.fileID = this.guid()
    this.sectionID = this.guid()
  },
  mounted() {
    i18nHelper.loadLanguagePack(4159)
  },
  methods: {
    upload() {
      $('#' + this.sectionID).fadeToggle()
    },
    UploadFileType() {
      this.$refs[this.fileID].value = ''
      this.$refs[this.fileID].click()
    },
    guid() {
      function S4() {
        return (((1 + Math.random()) * 0x10000) | 0).toString(16).substring(1)
      }
      return S4() + S4() + '-' + S4() + '-' + S4() + '-' + S4() + '-' + S4() + S4() + S4()
    },
    /*
     * 三个参数
     * file：一个是文件(类型是图片格式)，
     * w：一个是文件压缩的后宽度，宽度越小，字节越小
     * objDiv：一个是容器或者回调函数
     * photoCompress()
     */
    photoCompress(file, w, objDiv) {
      var self = this
      var ready = new FileReader()
      /*
        开始读取指定的Blob对象或File对象中的内容. 
        当读取操作完成时,readyState属性的值会成为DONE,
        如果设置了onloadend事件处理程序,则调用之.同时,
        result属性中将包含一个data: 
        URL格式的字符串以表示所读取文件的内容.
      */
      ready.readAsDataURL(file)
      ready.onload = function() {
        var re = this.result
        self.canvasDataURL(re, w, objDiv)
      }
    },
    canvasDataURL(path, obj, callback) {
      var img = new Image()
      img.src = path
      img.onload = function() {
        var that = this
        // 默认按比例压缩
        var w = that.width,
          h = that.height,
          scale = w / h
        w = obj.width || w
        h = obj.height || w / scale
        var quality = 0.7 // 默认图片质量为0.7
        //生成canvas
        var canvas = document.createElement('canvas')
        var ctx = canvas.getContext('2d')
        // 创建属性节点
        var anw = document.createAttribute('width')
        anw.nodeValue = w
        var anh = document.createAttribute('height')
        anh.nodeValue = h
        canvas.setAttributeNode(anw)
        canvas.setAttributeNode(anh)
        ctx.drawImage(that, 0, 0, w, h)
        // 图像质量
        if (obj.quality && obj.quality <= 1 && obj.quality > 0) {
          quality = obj.quality
        }
        // quality值越小，所绘制出的图像越模糊
        var base64 = canvas.toDataURL('image/jpeg', quality)
        // 回调函数返回base64的值
        callback(base64)
      }
    },
    /**
     * 将以base64的图片url数据转换为Blob
     * @param urlData
     * 用url方式表示的base64图片数据
     */
    convertBase64UrlToBlob(urlData) {
      var arr = urlData.split(','),
        mime = arr[0].match(/:(.*?);/)[1],
        bstr = atob(arr[1]),
        n = bstr.length,
        u8arr = new Uint8Array(n)

      while (n--) {
        u8arr[n] = bstr.charCodeAt(n)
      }

      return new Blob([u8arr], { type: mime })
    },
    //上传文件方法
    UpladFile() {
      var self = this
      try {
        var sourceType = [
          'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
          'application/pdf',
          'application/msword',
          'image/jpeg',
          'image/gif',
          'image/png',
          'image/jpg',
          'image/bmp',
        ]

        var notImageType = ['image/jpeg', 'image/gif', 'image/png', 'image/jpg', 'image/bmp']

        var fileObj = document.getElementById(this.fileID).files // js 获取文件对象
        var url =
          location.protocol + '//' + window.location.host + '/hr/onboard/upload_material/' + this.id + '/' + this.field // 接收上传文件的后台地址

        // 上传请假相关文件
        if (this.vacation) {
          url =
            location.protocol +
            '//' +
            window.location.host +
            '/attendance/upload_material/' +
            this.vacation +
            '/' +
            this.field // 接收上传文件的后台地址
        }
        //考勤相关附件
        if (this.abnormal) {
          url =
            location.protocol +
            '//' +
            window.location.host +
            '/attendance/upload_image/' +
            this.abnormal +
            '/' +
            this.field // 接收上传文件的后台地址
        }

        this.startDate = new Date().getTime()
        for (var item = 0; item < fileObj.length; item++) {
          this.showLoading = true
          if (this.uploadPDF && sourceType.indexOf(fileObj[item].type) == -1) {
            this.showLoading = false
            this.$message.error('只支持上传gif/jpeg/png/doc/pdf等类型的文件。')
            break
          }

          // 不允许上传pdf等文件限制
          if (!this.uploadPDF && notImageType.indexOf(fileObj[item].type) == -1) {
            this.showLoading = false
            this.$message.error('只允许上传图片类型的文件。')
            break
          }

          var form = new FormData() // FormData 对象

          if (fileObj[item].size / 1024 > 15360) {
            this.showLoading = false
            this.$message.error('您上传的文件过大，请上传15M以下的文件，15M以上请拍照上传或压缩后上传。')
            break
          }

          if (fileObj[item].size / 1024 > 1025 && notImageType.indexOf(fileObj[item].type) != -1) {
            //大于1M，进行压缩上传
            self.photoCompress(
              fileObj[item],
              {
                quality: 0.2,
              },
              function(base64Codes) {
                var bl = self.convertBase64UrlToBlob(base64Codes)
                form.append('file', bl, 'file_' + Date.parse(new Date()) + '.jpg') // 文件对象
                self.xhr = new XMLHttpRequest() // XMLHttpRequest 对象
                self.xhr.open('post', url, true) //post方式，url为服务器请求地址，true 该参数规定请求是否异步处理。
                self.xhr.onload = self.uploadComplete //请求完成
                self.xhr.onerror = self.uploadFailed //请求失败

                self.xhr.upload.onprogress = self.progressFunction //【上传进度调用方法实现】
                self.xhr.upload.onloadstart = function() {
                  //上传开始执行方法
                  self.ot = new Date().getTime() //设置上传开始时间
                  self.oloaded = 0 //设置上传开始时，以上传的文件大小为0
                }

                self.xhr.send(form) //开始上传，发送form数据
              }
            )
          } else {
            //小于等于1M 原图上传
            form.append('file', fileObj[item]) // 文件对象
            self.xhr = new XMLHttpRequest() // XMLHttpRequest 对象
            self.xhr.open('post', url, true) //post方式，url为服务器请求地址，true 该参数规定请求是否异步处理。
            self.xhr.onload = self.uploadComplete //请求完成
            self.xhr.onerror = self.uploadFailed //请求失败

            self.xhr.upload.onprogress = self.progressFunction //【上传进度调用方法实现】
            self.xhr.upload.onloadstart = function() {
              //上传开始执行方法
              self.ot = new Date().getTime() //设置上传开始时间
              self.oloaded = 0 //设置上传开始时，以上传的文件大小为0
            }

            self.xhr.send(form) //开始上传，发送form数据
          }
        }
      } catch (e) {
        this.$message.error('上传失败。')
        this.showLoading = false
      }
    },

    //上传成功响应
    uploadComplete(evt) {
      //服务断接收完文件返回的结果
      this.showLoading = false
      var data = JSON.parse(evt.target.responseText)
      this.startDate = new Date().getTime()
      if (data.code == 0) {
        this.$message.success('上传成功。')
        if (data.field == 'identity_card_positive' || data.field == 'identity_card_back') {
          this.$emit(
            'success',
            {
              id: data.data.id,
              src: data.data.pre_url,
              url: data.data.filepath,
              data: data,
              extension: data.data.file_extension,
            },
            data.field
          )
        } else {
          this.$emit(
            'success',
            {
              id: data.data.id,
              src: data.data.pre_url,
              url: data.data.filepath,
              extension: data.data.file_extension,
            },
            data.field
          )
        }
      } else {
        let errMessage = '上传失败。'
        if (data.message) {
          errMessage = data.message
        }

        this.$message.error(errMessage)
      }
      $(this.fileID + this.isPhoto).val('')
    },
    //上传失败
    uploadFailed(evt) {
      this.$message.error('上传失败。')
      this.showLoading = false
    },
    //取消上传
    cancleUploadFile() {
      this.xhr.abort()
    },
    //上传进度实现方法，上传过程中会频繁调用该方法
    progressFunction(evt) {
      this.showLoading = true

      this.endDate = new Date().getTime()
      if ((parseInt(this.endDate) - parseInt(this.startDate)) / 1000 > 60) {
        this.$message.error('上传超时，请刷新界面，检查网络是否正常，如上传PDF文件大于10M，请压缩后上传。')
        this.showLoading = false
        return false
      }

      /*
        var progressBar = document.getElementById("progressBar");
        var percentageDiv = document.getElementById("percentage");
        // event.total是需要传输的总字节，event.loaded是已经传输的字节。如果event.lengthComputable不为真，则event.total等于0
        if (evt.lengthComputable) {//
            progressBar.max = evt.total;
            progressBar.value = evt.loaded;
            percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100) + "%";
        }
        var time = document.getElementById("time");
        var nt = new Date().getTime();//获取当前时间
        var pertime = (nt-self.ot)/1000; //计算出上次调用该方法时到现在的时间差，单位为s
        self.ot = new Date().getTime(); //重新赋值时间，用于下次计算
        var perload = evt.loaded - self.oloaded; //计算该分段上传的文件大小，单位b
        self.oloaded = evt.loaded;//重新赋值已上传文件大小，用以下次计算
        //上传速度计算
        var speed = perload/pertime;//单位b/s
        var bspeed = speed;
        var units = 'b/s';//单位名称
        if(speed/1024>1){
            speed = speed/1024;
            units = 'k/s';
        }
        if(speed/1024>1){
            speed = speed/1024;
            units = 'M/s';
        }
        speed = speed.toFixed(1);
        //剩余时间
        var resttime = ((evt.total-evt.loaded)/bspeed).toFixed(1);
        time.innerHTML = '，速度：'+speed+units+'，剩余时间：'+resttime+'s';
        if(bspeed==0) time.innerHTML = '上传已取消';
        */
    },
  },
}
</script>
<style>
.el-message {
  z-index: 20000 !important;
  top: 0px !important;
}
.weui-cell {
  padding: 10px 15px;
  position: relative;
  display: -webkit-box;
  display: -ms-flexbox;
  display: flex;
  -webkit-box-align: center;
  -ms-flex-align: center;
  align-items: center;
}
.weui-cells {
  margin-top: 1.17647059em;
  background-color: #ffffff;
  line-height: 1.41176471;
  font-size: 17px;
  overflow: hidden;
  position: relative;
}
.weui-cell:before {
  content: ' ';
  position: absolute;
  left: 0;
  top: 0;
  right: 0;
  height: 1px;
  border-top: 1px solid #d9d9d9;
  color: #d9d9d9;
  -webkit-transform-origin: 0 0;
  transform-origin: 0 0;
  -webkit-transform: scaleY(0.5);
  transform: scaleY(0.5);
  left: 15px;
}
.weui-cell:first-child:before {
  display: none;
}
.el-upload {
  display: inline-block;
  text-align: right;
  cursor: pointer;
  outline: 0;
}
.weui-cell__ft {
  text-align: right;
  color: #999999;
}
.vux-cell-primary {
  -webkit-box-flex: 1;
  -ms-flex: 1;
  flex: 1;
}
#pushbuttonClick,
#refresh {
  position: absolute;
  top: 50%;
  width: 100%;
  margin-top: -50px;
  font-size: 30px;
  text-align: center;
  color: #333;
}
#refresh {
  margin-top: -100px;
}
#pushbuttonClick:active {
  color: #000;
}
/*pushbutton*/
.widget-ui-pushbutton {
  position: absolute;
  top: 0;
  left: 0;
  z-index: 100;
  width: 100%;
  height: 100%;
}
.widget-ui-pushbutton-list {
  position: absolute;
  left: 0;
  bottom: 0;
  z-index: 101;
  width: 100%;
  overflow-y: auto;
  background: #fff;
  text-align: center;
  color: #333;
}
.widget-ui-pushbutton-list > a.pushbutton-cancel,
.widget-ui-pushbutton-list > a.list-a {
  display: block;
  padding: 15px;
  border-top: 1px solid #d4d4d4;
  font-size: 16px;
  color: #333;
  cursor: pointer;
  white-space: nowrap;
  text-overflow: ellipsis;
  overflow: hidden;
}
.widget-ui-pushbutton-list > a.pushbutton-cancel:active,
.widget-ui-pushbutton-list > a.list-a:active {
  background: #ccc;
  color: #fff;
}
.widget-ui-pushbutton-list > a.list-a:first-child {
  border: none;
}
.widget-ui-pushbutton-list > a.pushbutton-cancel {
  color: #6da0f6;
}
.pushbutton-in {
  display: block;
  animation: pushbuttonIn 0.35s forwards;
  -webkit-animation: pushbuttonIn 0.35s forwards;
}
@keyframes pushbuttonIn {
  from {
    background: rgba(0, 0, 0, 0);
  }
  to {
    background: rgba(0, 0, 0, 0.5);
  }
}
@-webkit-keyframes pushbuttonIn {
  from {
    background: rgba(0, 0, 0, 0);
  }
  to {
    background: rgba(0, 0, 0, 0.5);
  }
}
.pushbutton-out {
  animation: pushbuttonOut 0.35s forwards;
  -webkit-animation: pushbuttonOut 0.35s forwards;
}
@keyframes pushbuttonOut {
  from {
    background: rgba(0, 0, 0, 0.5);
  }
  to {
    background: rgba(0, 0, 0, 0);
  }
}
@-webkit-keyframes pushbuttonOut {
  from {
    background: rgba(0, 0, 0, 0.5);
  }
  to {
    background: rgba(0, 0, 0, 0);
  }
}
.pushbutton-in .widget-ui-pushbutton-list {
  animation: pushbuttonUlIn 0.35s forwards;
  -webkit-animation: pushbuttonUlIn 0.35s forwards;
}
@keyframes pushbuttonUlIn {
  from {
    -webkit-transform: translate(0, 100%) translateZ(0);
    transform: translate(0, 100%) translateZ(0);
  }
  to {
    -webkit-transform: translate(0, 0) translateZ(0);
    transform: translate(0, 0) translateZ(0);
  }
}
@-webkit-keyframes pushbuttonUlIn {
  from {
    -webkit-transform: translate(0, 100%) translateZ(0);
    transform: translate(0, 100%) translateZ(0);
  }
  to {
    -webkit-transform: translate(0, 0) translateZ(0);
    transform: translate(0, 0) translateZ(0);
  }
}
.pushbutton-out .widget-ui-pushbutton-list {
  animation: pushbuttonUlOut 0.35s forwards;
  -webkit-animation: pushbuttonUlOut 0.35s forwards;
}
@keyframes pushbuttonUlOut {
  from {
    -webkit-transform: translate(0, 0) translateZ(0);
    transform: translate(0, 0) translateZ(0);
  }
  to {
    -webkit-transform: translate(0, 100%) translateZ(0);
    transform: translate(0, 100%) translateZ(0);
  }
}
@-webkit-keyframes pushbuttonUlOut {
  from {
    -webkit-transform: translate(0, 0) translateZ(0);
    transform: translate(0, 0) translateZ(0);
  }
  to {
    -webkit-transform: translate(0, 100%) translateZ(0);
    transform: translate(0, 100%) translateZ(0);
  }
}

#uploader .vux-flexbox .vux-flexbox-item {
  width: 120px;
  height: 90px;
}
#uploader .el-upload-list--picture-card .el-upload-list__item {
  margin: 0px 0px 0px 0px;
}
.vux-search-box,
.dp-container,
.dp-mask,
.vux-search_show,
.vux-popup-mask,
.vux-popup-dialog {
  max-width: 640px !important;
  margin: 0 auto !important;
  width: 100% !important;
  right: 0 !important;
}
#uploader hr {
  padding: 0px !important;
  margin: 0px !important;
  margin-top: 0px !important;
  margin-bottom: 0px !important;
  border-top: 0px !important;
}
#uploader .weui-cells:after,
#upload .weui-cells:before,
#upload .weui-cell:before,
#upload .weui-cell:after,
#upload .weui-cell,
#upload weui-cells {
  border: none !important;
}
.weui-search-bar__form:after {
  border: 0px !important;
}
.weui-search-bar:after {
  border-bottom: 0px !important;
}
.weui-search-bar__box {
  padding-left: 2px !important;
  padding-right: 0px !important;
}
.weui-search-bar {
  background-color: #ffffff !important;
}
.weui-search-bar__cancel-btn {
  padding-top: 10px !important;
}
.weui-icon-search {
  display: none !important;
}
.weui-search-bar__box .weui-search-bar__input {
  padding: 0px !important;
  font-upload: PingFangSC-Regular !important;
  font-size: 16px !important;
  color: #000000 !important;
  text-align: left !important;
}
.weui-search-bar__box .weui-icon-clear {
  display: none !important;
}
.weui-search-bar__form:after {
  border-radius: 0px !important;
}
#uploader .weui-cell {
  padding: 9px 28px !important;
  height: 48px;
  line-height: 48px;
}
#uploader .weui-input {
  height: 30px !important;
  line-height: 30px !important;
  width: 173px !important;
}
#uploader .weui_icon_clear {
  padding-right: 30px;
  padding-left: 0px;
  margin-left: 0px;
}
#uploader .weui-cells {
  margin-top: 0px !important;
  padding: 0px !important;
  font-size: 1.35rem !important;
}
#uploader .weui-cells p {
  margin: 0px !important;
}

#uploader .vux-no-group-title {
  margin-top: 0px !important;
}
.el-message {
  z-index: 20000 !important;
  top: 0px !important;
}
#uploader .weui-cell_access .weui-cell__ft {
  display: inline-block !important;
  width: 55% !important;
  text-align: left;
}
#uploader .weui-cell__ft > .vux-cell-value {
  display: inline-block;
  width: 100%;
  text-align: left;
}
#uploader .weui-cell_access .weui-cell__ft:after {
  margin-top: -2px !important;
  right: -14px !important;
}
#uploader .weui-cell__ft {
  text-align: right !important;
  color: #999999 !important;
  font-upload: PingFang-SC-Medium !important;
  font-size: 16px !important;
  color: #000000 !important;
  letter-spacing: 0 !important;
}
#uploader .vux-cell-value {
  font-upload: PingFang-SC-Medium !important;
  font-size: 16px !important;
  color: #000000 !important;
  letter-spacing: 0 !important;
  white-space: nowrap !important;
  padding-left: 1rem !important;
}
#uploader .vux-cell-primary label {
  margin-bottom: 0px !important;
  font-weight: normal !important;
}
.weui-check__label .weui-cell__bd {
  height: 22px;
  line-height: 22px;
}
label {
  margin-bottom: 0px !important;
}
#uploader .vux-label {
  opacity: 0.5 !important;
  font-upload: PingFang-SC-Regular !important;
  font-size: 16px !important;
  color: #000000 !important;
  letter-spacing: 0 !important;
}
#uploader .vux-cell-primary {
  flex: none !important;
  -webkit-box-flex: none !important;
  width: 38% !important;
}
#uploader .vux-datetime > div {
  display: inline-block !important;
  width: 38% !important;
}
#uploader .vux-datetime > div > p {
  display: inline-block !important;
  margin-block-start: 1em;
  margin-block-end: 1em;
  margin-inline-start: 0px;
  margin-inline-end: 0px;
  word-wrap: break-word;
  word-break: break-all;
  opacity: 0.5 !important;
  font-upload: PingFang-SC-Regular !important;
  font-size: 16px !important;
  color: #000000 !important;
  letter-spacing: 0 !important;
  max-width: 100% !important;
  margin-bottom: 5px;
  width: 100% !important;
}
#multipleInput .weui-cell {
  height: 92px;
  position: relative;
}
#multipleInput .weui-cell .vux-cell-primary p {
  position: absolute;
  top: 3px;
}
#multipleInput .weui-cell__bd textarea {
  line-height: 22px;
}
#multipleInput .vux-x-textarea {
  padding: 13px !important;
}
#friendInput .vux-cell-primary {
  width: 74% !important;
}
#friendNameInput .vux-cell-primary {
  width: 60% !important;
}
#friendNameInput .weui-input {
  width: 100px !important;
}
#friendInput .weui-cell_access .weui-cell__ft:after {
  margin-top: -2px !important;
  right: 7px !important;
}
#relationInput .vux-cell-primary {
  width: 74% !important;
}
#relationInput .weui-cell__ft {
  position: absolute;
  right: 15px;
}
#relationInput .weui-cell_access .weui-cell__ft:after {
  margin-top: -2px !important;
  right: 20px !important;
}
#uploader .vux-datetime {
  display: block !important;
  width: 100% !important;
  padding: 0px 32px 0px 28px !important;
  margin: 0px !important;
}
#uploader .preview-container {
  font-upload: PingFangSC-Semibold;
  width: 100%;
  margin: 0 auto;
  font-size: 14px;
  background: #f4f4f4;
  margin-top: 45px;
}
#uploader .preview-list {
  margin-top: 20px;
  background: #ffffff;
  padding-bottom: 20px;
}
#uploader .preview-title {
  color: #4899ff;
  letter-spacing: 1px;
  padding-bottom: 5px;
  border-bottom: 1px solid #d8d8d8;
  margin-bottom: 10px;
  width: 85%;
  margin: 0 auto;
  padding-top: 15px;
}
#uploader .preview-flex-left {
  width: 90%;
  text-align: left;
  font-upload: PingFang-SC-Regular;
  line-height: 28px;
  color: #848484;
}
#uploader .preview-flex-right {
  width: 90%;
  line-height: 28px;
  color: #000000;
}
#uploader .preview-flex-pic {
  height: 90px;
  width: 90%;
  background: #f6f6f6;
  color: #dddddd;
  text-align: center;
  font-size: 12px;
}
#uploader .preview-flex-pic img {
  margin-top: 30px;
  margin-bottom: 5px;
}
#uploader .preview-error {
  background: #ff9d19;
}
#uploader .preview-error .preview-flex-left {
  color: #975d0e;
}
#uploader .preview-bottom {
  text-align: center;
  background-color: #ffffff;
  font-size: 16px;
  color: #4899ff;
  padding: 10px 8px 10px 0px;
  width: 100%;
  height: 60px;
  position: fixed;
  bottom: 0;
  max-width: 640px;
  margin: 0 auto;
  display: inline-block;
  border-top: 1px solid #e5e5e5;
}
#uploader .preview-bottom div {
  width: 100%;
  text-align: center;
}
#uploader .bottom-button {
  background: #879bb5;
  border-radius: 4px;
  color: #e7ebf0;
  opacity: 0.9;
  opacity: 0.9;
  font-upload: PingFang-SC-Medium;
  font-size: 16px;
  color: #ffffff;
  letter-spacing: 0;
  font-weight: normal;
}
#uploader .vux-x-input {
  padding: 0px 15px !important;
}
#uploader .el-input__inner {
  border: 1px solid #fff !important;
  color: #000 !important;
  padding-right: 34px !important;
  font-upload: PingFang-SC-Medium !important;
  font-size: 16px !important;
  letter-spacing: 0 !important;
  height: 22px !important;
  line-height: 22px !important;
}
#uploader .el-input__icon {
  width: 0px !important;
}
.vux-popup-dialog .weui-cell {
  height: 48px !important;
  line-height: 48px !important;
  padding: 10px 28px !important;
  font-upload: PingFang-SC-Regular !important;
  font-size: 16px !important;
  color: #000000 !important;
  letter-spacing: 0 !important;
}
.vux-popup-dialog .weui-cell:before {
  left: 28px !important;
  right: 28px !important;
}
#uploader .el-select__caret {
  display: none;
}
#uploader .widget-ui-pushbutton {
  position: fixed !important;
  top: 0 !important;
  left: 0 !important;
  z-index: 100 !important;
  width: 100% !important;
  height: 100% !important;
  overflow: hidden !important;
}
.weui-search-bar__cancel-btn {
  color: #4899ff !important;
}
.search_padding > .weui-cell {
  padding: 0px 15px !important;
}
.el-message {
  z-index: 20000 !important;
  top: 0px !important;
}
</style>

