$(document).ready(function(){
  const btnOnWS  = $('#on-websocket')
  const btnOffWS = $('#off-websocket')

  const btnWS_ID       = $('#websocket-ID')
  const btnWS_IDSN     = $('#websocket-ID-sn')
  const btnWS_ASN      = $('#websocket-A-sn')
  const btnWS_DeviceNo = $('#websocket-device-No')

  const btnHttp_ID       = $('#http-ID')
  const btnHttp_IDSN     = $('#http-ID-sn')
  const btnHttp_ASN      = $('#http-A-sn')
  const btnHttp_DeviceNo = $('#http-device-No')

  const connectAddress  = $('#connect-address')
  const processContent  = $('#process-content')
  const btnCleanProcess = $('#clean-process')

  const cardFront  = $('#card-front')
  const cardBack   = $('#card-back')

  const SNContent  = $('#SN-content')
  const image      = $('#image')
  const name       = $('#name')
  const sex        = $('#sex')
  const nation     = $('#nation')
  const year       = $('#year')
  const month      = $('#month')
  const date       = $('#date')
  const address    = $('#address')
  const number     = $('#number')
  const department = $('#department')
  const expiry     = $('#expiry')

  let ws = null



  btnOnWS.on('click', conWS)
  btnOffWS.on('click', disconWS)

  btnWS_ID.on('click', WS_ReadInfo)
  btnWS_IDSN.on('click', WS_GetBCardNo)
  btnWS_ASN.on('click', WS_GetASN)
  btnWS_DeviceNo.on('click', WS_GetDeviceNo)

  btnHttp_ID.on('click', Http_ReadInfo)
  btnHttp_IDSN.on('click', Http_GetBCardNo)
  btnHttp_ASN.on('click', Http_GetASN)
  btnHttp_DeviceNo.on('click', Http_GetDeviceNo)

  btnCleanProcess.on('click', cleanMsg)

  processContent.text('本demo支持websocket和webapi两种网页调用方式')

  function hex2a(hex) {
    let str_list = ''
    for (let i = 0; i < hex.length && hex.substr(i, 2) !== '00'; i += 2) {
      const a = hex.charCodeAt(i)
      const b = hex.charCodeAt(i + 1)
      const c = b * 256 + a
      str_list += String.fromCharCode(c)
    }

    return str_list.toString()
  }

  function setDocumentInfo(szparam)
  {
    if (szparam.CardType == 74)
    {
      // 切换背景图片   83是台湾
      cardFront.removeClass()
      cardBack.removeClass()
      cardFront.addClass('GAT-card')
      cardFront.addClass('card-hongkong-macao-taiwan-front')
      cardBack.addClass('GAT-card')
      let no = hex2a(window.atob(szparam.CardInfo.No))
      if (no && no.startsWith('83')){
        cardBack.addClass('card-taiwan-back')
      }else{
        cardBack.addClass('card-hongkong-macao-back')
      }
      strLog = '读取 港澳台居民居住证 成功\r\n';
      strLog += 'SN：' + szparam.CardInfo.SN + '\r\n';
      strLog += '中文名：' + hex2a(window.atob(szparam.CardInfo.Name)) + '\r\n';
      strLog += '证件号码：' + no + '\r\n';
      strLog += '性别：' + hex2a(window.atob(szparam.CardInfo.Sex)) + '\r\n';
      strLog += '出生日期：' + hex2a(window.atob(szparam.CardInfo.Birthday)) + '\r\n';
      strLog += '民族：' + hex2a(window.atob(szparam.CardInfo.Nation)) + '\r\n';
      strLog += '地址：' + hex2a(window.atob(szparam.CardInfo.Address)) + '\r\n';
      strLog += '签发机关：' + hex2a(window.atob(szparam.CardInfo.SignedDepartment)) + '\r\n';
      strLog += '开始日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin)) + '\r\n';
      strLog += '结束日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)) + '\r\n\r\n';
      strLog += '通行证号码：' + hex2a(window.atob(szparam.CardInfo.OtherNO)) + '\r\n';
      strLog += '签发次数：' + hex2a(window.atob(szparam.CardInfo.SignNum)) + '\r\n';
      processContent.text(strLog)

      // 港澳台通行证号码
      console.log(szparam.CardInfo)
      cardFront.find('.name').text(hex2a(window.atob(szparam.CardInfo.Name)))
      cardFront.find('.sex').text(hex2a(window.atob(szparam.CardInfo.Sex)) ==='1'? '男':'女')

      const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
      const birthArr = parseDateString(Birthday , ".", true).split(".")
      cardFront.find('.year').text(birthArr[0])
      cardFront.find('.month').text(birthArr[1])
      cardFront.find('.date').text(birthArr[2])

      cardFront.find('.address').text(hex2a(window.atob(szparam.CardInfo.Address)))
      cardFront.find('.number').text(hex2a(window.atob(szparam.CardInfo.No)))
      cardBack.find('.department').text(hex2a(window.atob(szparam.CardInfo.SignedDepartment)))
      const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
      const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
      const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
      const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
      cardBack.find('.expiry').text( expiryBegin + '-' + expiryEnd)
      cardBack.find('.pass-number').text(  hex2a(window.atob(szparam.CardInfo.OtherNO)))
    }
    else if (szparam.CardType == 73)
    {
      // 切换背景图片   83是台湾
      cardFront.removeClass()
      cardBack.removeClass()
      cardFront.addClass('WGR-card-1')
      cardFront.addClass('card-old-foreigner-front')
      cardBack.addClass('WGR-card-1')
      cardBack.addClass('card-old-foreigner-back')

      strLog = '读取 外国人永久居留身份证(旧版) 成功\r\n';
      strLog += 'SN：' + szparam.CardInfo.SN + '\r\n';
      strLog += '中文名：' + hex2a(window.atob(szparam.CardInfo.Name)) + '\r\n';
      strLog += '英文名：' + hex2a(window.atob(szparam.CardInfo.EnName)) + '\r\n';
      strLog += '证件号码：' + hex2a(window.atob(szparam.CardInfo.No)) + '\r\n';
      strLog += '性别：' + hex2a(window.atob(szparam.CardInfo.Sex)) + '\r\n';
      strLog += '出生日期：' + hex2a(window.atob(szparam.CardInfo.Birthday)) + '\r\n';
      strLog += '国籍：' + hex2a(window.atob(szparam.CardInfo.Country)) + '\r\n';
      strLog += '签发机关：中华人民共和国移民管理局\r\n';
      strLog += '开始日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin)) + '\r\n';
      strLog += '结束日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)) + '\r\n\r\n';
      strLog += '版本号：' + hex2a(window.atob(szparam.CardInfo.Version)) + '\r\n';
      processContent.text(strLog)

      let name = hex2a(window.atob(szparam.CardInfo.Name))
      let enName = hex2a(window.atob(szparam.CardInfo.EnName))
      let nameText = enName + (name.trim()? ' / '+ name : '')
      cardFront.find('.name').text(nameText)
      cardFront.find('.sex').text(hex2a(window.atob(szparam.CardInfo.Sex)) === '1'? '男': '女')
      const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
      const birthArr = parseDateString(Birthday , ".", true).split(".")
      cardFront.find('.year').text(birthArr.join('-')) //出生年月
      cardFront.find('.month').text(hex2a(window.atob(szparam.CardInfo.Country)))//国籍
      const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
      const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
      const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
      const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
      cardFront.find('.date').text(expiryBegin + '-' + expiryEnd)
      cardFront.find('.address').text('中华人民共和国移民管理局') //
      cardFront.find('.number').text(hex2a(window.atob(szparam.CardInfo.No)))
    }
    else if (szparam.CardType == 89)
    {
      // 切换背景图片   83是台湾
      cardFront.removeClass()
      cardBack.removeClass()
      cardFront.addClass('WGR-card')
      cardFront.addClass('card-new-foreigner-front')
      cardBack.addClass('WGR-card')
      cardBack.addClass('card-new-foreigner-back')

      strLog = '读取 外国人永久居留身份证(新版) 成功\r\n';
      strLog += 'SN：' + szparam.CardInfo.SN + '\r\n';
      strLog += '中文名：' + hex2a(window.atob(szparam.CardInfo.Name)) + '\r\n';
      strLog += '英文名：' + hex2a(window.atob(szparam.CardInfo.EnName)) + '\r\n';
      strLog += '证件号码：' + hex2a(window.atob(szparam.CardInfo.No)) + '\r\n';
      strLog += '性别：' + hex2a(window.atob(szparam.CardInfo.Sex)) + '\r\n';
      strLog += '出生日期：' + hex2a(window.atob(szparam.CardInfo.Birthday)) + '\r\n';
      strLog += '国籍：' + hex2a(window.atob(szparam.CardInfo.Country)) + '\r\n';
      strLog += '签发机关：中华人民共和国移民管理局\r\n';
      strLog += '开始日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin)) + '\r\n';
      strLog += '结束日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)) + '\r\n\r\n';
      strLog += '通行证号码：' + hex2a(window.atob(szparam.CardInfo.OtherNO)) + '\r\n';
      strLog += '签发次数：' + hex2a(window.atob(szparam.CardInfo.SignNum)) + '\r\n';
      processContent.text(strLog)
      let name = hex2a(window.atob(szparam.CardInfo.Name))
      let enName = hex2a(window.atob(szparam.CardInfo.EnName))
      cardFront.find('.name').text(name)
      cardFront.find('.enName').text(enName)
      cardFront.find('.sex').text(hex2a(window.atob(szparam.CardInfo.Sex)) === '1'? '男': '女')
      const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
      const birthArr = parseDateString(Birthday , ".", true).split(".")
      cardFront.find('.year').text(birthArr.join('-')) //出生年月
      cardFront.find('.month').text(hex2a(window.atob(szparam.CardInfo.Country)))//国籍
      const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
      const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
      const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
      const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
      cardFront.find('.date').text(expiryBegin + '-' + expiryEnd)
      cardFront.find('.number').text(hex2a(window.atob(szparam.CardInfo.No)))
    }
    else
    {
      cardFront.removeClass()
      cardBack.removeClass()
      cardFront.addClass('ID-card')
      cardFront.addClass('card-front')
      cardBack.addClass('ID-card')
      cardBack.addClass('card-back')
      strLog = '读取 身份证 成功\r\n';
      strLog += 'SN：' + szparam.CardInfo.SN + '\r\n';
      strLog += '中文名：' + hex2a(window.atob(szparam.CardInfo.Name)) + '\r\n';
      strLog += '证件号码：' + hex2a(window.atob(szparam.CardInfo.No)) + '\r\n';
      strLog += '性别：' + hex2a(window.atob(szparam.CardInfo.Sex)) + '\r\n';
      strLog += '出生日期：' + hex2a(window.atob(szparam.CardInfo.Birthday)) + '\r\n';
      strLog += '民族：' + hex2a(window.atob(szparam.CardInfo.Nation)) + '\r\n';
      strLog += '地址：' + hex2a(window.atob(szparam.CardInfo.Address)) + '\r\n';
      strLog += '签发机关：' + hex2a(window.atob(szparam.CardInfo.SignedDepartment)) + '\r\n';
      strLog += '开始日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin)) + '\r\n';
      strLog += '结束日期：' + hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)) + '\r\n\r\n';
      strLog += '通行证号码：' + hex2a(window.atob(szparam.CardInfo.OtherNO)) + '\r\n';
      strLog += '签发次数：' + hex2a(window.atob(szparam.CardInfo.SignNum)) + '\r\n';
      processContent.text(strLog)
    //  内容填充
      cardFront.find('.name').text(hex2a(window.atob(szparam.CardInfo.Name)))
      cardFront.find('.sex').text(hex2a(window.atob(szparam.CardInfo.Sex)))
      cardFront.find('.nation').text(hex2a(window.atob(szparam.CardInfo.Nation)))

      const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
      const birthArr = parseDateString(Birthday , ".", true).split(".")
      cardFront.find('.year').text(birthArr[0])
      cardFront.find('.month').text(birthArr[1])
      cardFront.find('.date').text(birthArr[2])

      cardFront.find('.address').text(hex2a(window.atob(szparam.CardInfo.Address)))
      cardFront.find('.number').text(hex2a(window.atob(szparam.CardInfo.No)))
      cardBack.find('.department').text(hex2a(window.atob(szparam.CardInfo.SignedDepartment)))
      const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
      const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
      const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
      const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
      cardBack.find('.expiry').text( expiryBegin + '-' + expiryEnd)
    }
    SNContent.text(szparam.CardInfo.SN)
    cardFront.find('.image').attr('src','data:image/jpg;base64,' + szparam.BmpInfo)
    // if (szparam.CardInfo.Name){
    //   cardFront.find('.name').text(hex2a(window.atob(szparam.CardInfo.Name)))
    // }
    // if (szparam.CardInfo.Sex){
    //   cardFront.find('.sex').text(hex2a(window.atob(szparam.CardInfo.Sex)))
    // }
    // if (szparam.CardInfo.Nation){
    //   cardFront.find('.nation').text(hex2a(window.atob(szparam.CardInfo.Nation)))
    // }
    //
    // const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
    // const birthArr = parseDateString(Birthday , ".", true).split(".")
    // cardFront.find('.year').text(birthArr[0])
    // cardFront.find('.month').text(birthArr[1])
    // cardFront.find('.date').text(birthArr[2])
    //
    // if (szparam.CardInfo.Address){
    //   cardFront.find('.address').text(hex2a(window.atob(szparam.CardInfo.Address)))
    // }
    //
    // if (szparam.CardInfo.No){
    //   cardFront.find('.number').text(hex2a(window.atob(szparam.CardInfo.No)))
    // }
    //
    // if (szparam.CardInfo.SignedDepartment){
    //   cardBack.find('.department').text(hex2a(window.atob(szparam.CardInfo.SignedDepartment)))
    // }
    //
    // const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
    // const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
    // const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
    // const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
    // cardBack.find('.expiry').text( expiryBegin + '-' + expiryEnd)

  }

  function conWS() {
    const webUrl = 'ws://' + connectAddress.val() + '/ws'
    ws = new WebSocket(webUrl)
    ws.onopen = function (evt) {
      let szhelp = 'websocket连接成功，url[' + webUrl + ']，读卡器上放置身份证后websocket会自动接收身份证数据，如需手动操作请调用WS_ReadInfo()函数\r\n\r\n'
      szhelp += '支持被动接收和主动请求两种方式\r\n'
      szhelp += '被动接收:当读卡器刷卡成功后会推送身份证信息到websocket，websocket直接显示即可\r\n'
      szhelp += '主动请求:支持网页端主动向服务器请求对应的消息。可查看<WS_ReadInfo><WS_GetASN><WS_GetBCardNo>这三个接口'

      processContent.text(szhelp)
    }
    ws.onclose = function (evt) {
      processContent.text('websocket已断开')
    }
    ws.onmessage = function (messageEvent) {
      const jsonobject = JSON.parse(messageEvent.data)
      if (jsonobject.Ret == 0) {
        if (jsonobject.Cmd == 10001) {
          cleanMsg()
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          setDocumentInfo(szparam);
        } else if (jsonobject.Cmd == 30401) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读取A卡SN成功：' + szparam.SN)
        } else if (jsonobject.Cmd == 20401) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读取身份证卡片SN成功：' + szparam.SN)
        } else if (jsonobject.Cmd == 20511) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读卡器唯一号：' + szparam.SN)
        }else if (jsonobject.Cmd == 1000) {
            szparam = JSON.parse(window.atob(jsonobject.UserParam));
            if (szparam.State == 0)
            {
          	processContent.text('读卡器已被拔出')
            }
            else processContent.text('读卡器已插入')
          }
      } else {
        processContent.text('websocket 协议调用失败，原因：' + jsonobject.ErrInfo)
      }
    }
  }

  function disconWS() {
    if (ws) {
      ws.close()
      processContent.text('websocket已断开')
    }
  }

  function cleanMsg() {
    processContent.text('')
    SNContent.text('')
    cardFront.find('.name').text('')
    cardFront.find('.enName').text('')
    cardFront.find('.sex').text('')
    cardFront.find('.nation').text('')
    cardFront.find('.year').text('')
    cardFront.find('.month').text('')
    cardFront.find('.date').text('')
    cardFront.find('.address').text('')
    cardFront.find('.number').text('')
    cardBack.find('.department').text('')
    cardBack.find('.expiry').text('')
    cardBack.find('.pass-number').text('')
    cardFront.find('.image').attr('src', '')
  }

  function WS_GetASN() {
    const szJson = '{"Cmd":30400,"Head":"YZWL","IPFlag":"YWYyNWMxOWQ1ZTY4ZmJhOQ==","UserParam":"","Version":"V1.0.0"}\n'
    ws.send(szJson)
  }

  function WS_GetBCardNo() {
    const szJson = '{"Cmd":20400,"Head":"YZWL","IPFlag":"YWYyNWMxOWQ1ZTY4ZmJhOQ==","UserParam":"","Version":"V1.0.0"}\n'
    ws.send(szJson)
  }
  function WS_GetDeviceNo() {
    const szJson = '{"Cmd":20510,"Head":"YZWL","IPFlag":"YWYyNWMxOWQ1ZTY4ZmJhOQ==","UserParam":"","Version":"V1.0.0"}\n'
    ws.send(szJson)
  }

  function WS_ReadInfo() {
    cleanMsg()
    const szJson = '{"Cmd":10000,"Head":"YZWL","IPFlag":"MGEyZmU1NmY5ODZlZGMyNg==","UserParam":"eyJBcHBLZXkiOiI5OWZmYjJmOThhMjkwNzExMDdjN2EwOWFkMmM2ZDA5NiIsIkRlY29kZVBob3RvIjp0cnVlLCJGYWNlQ29tcGFyZSI6ZmFsc2UsIlBob3RvRm9ybWF0IjoxLCJTZXJ2ZXJJUCI6ImlkLnl6ZnV0dXJlLmNuIiwiU2VydmVyUG9ydCI6ODg0OH0NCg==","Version":"V1.0.0"}\n'
    ws.send(szJson)
  }

  function Http_ReadInfo() {
    cleanMsg()
    const webUrl = 'http://' + connectAddress.val() + '/api/info'
    $.ajax({
      url: webUrl,
      type: 'GET',
      dataType: 'json',
      success: function (result) {
        processContent.text('web api接口：' + webUrl + ' 读取身份证信息成功')
        const szparam = result
        setDocumentInfo(szparam);
      },
      error: function (jqXHR, textStatus, errorThrown) {
        processContent.text('web api接口：' + webUrl + ' 读取身份证失败，原因:' + hex2a(window.atob(errorThrown)))
      }
    })
  }
  function Http_GetASN() {
    const webUrl = 'http://' + connectAddress.val() + '/api/asn'
    $.ajax({
      url: webUrl,
      type: 'GET',
      dataType: 'json',
      success: function (result) {
        const szparam = result
        processContent.text('web api接口：' + webUrl + ' 读取成功。 A卡SN：' + szparam.SN)
      },
      error: function (jqXHR, textStatus, errorThrown) {
        processContent.text('web api接口：' + webUrl + ' 读取A卡SN失败，原因:' + hex2a(window.atob(errorThrown)))
      }
    })
  }
  function Http_GetBCardNo() {
    const webUrl = 'http://' + connectAddress.val() + '/api/bsn'
    $.ajax({
      url: webUrl,
      type: 'GET',
      dataType: 'json',
      success: function (result) {
        const szparam = result
        processContent.text('web api接口：' + webUrl + ' 读取成功。 身份证卡片SN：' + szparam.SN)
      },
      error: function (jqXHR, textStatus, errorThrown) {
        processContent.text('web api接口：' + webUrl + ' 读取身份证卡片SN失败，原因:' + hex2a(window.atob(errorThrown)))
      }
    })
  }
  function Http_GetDeviceNo() {
    const webUrl = 'http://' + connectAddress.val() + '/api/devsn'
    $.ajax({
      url: webUrl,
      type: 'GET',
      dataType: 'json',
      success: function (result) {
        const szparam = result
        processContent.text('web api接口：' + webUrl + ' 读取成功。 读卡器芯唯一号：' + szparam.SN)
      },
      error: function (jqXHR, textStatus, errorThrown) {
        processContent.text('web api接口：' + webUrl + ' 读取读卡器芯唯一号失败，原因:' + hex2a(window.atob(errorThrown)))
      }
    })
  }

  function parseDateString(str, deco, zero) {
    let year = str.substr(0,4)
    let month = str.substr(4,2)
    let date = str.substr(6)
    if(zero) {
      month = month.substr(0,1) === "0" ? month.substr(1) : month
      date = date.substr(0,1) === "0" ? date.substr(1) : date
    }
    return `${year}${deco}${month}${deco}${date}`
  }
})
