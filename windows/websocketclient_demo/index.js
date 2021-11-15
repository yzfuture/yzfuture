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
          processContent.text('websocket 协议 读取身份证成功')
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))

          SNContent.text(szparam.CardInfo.SN)
          name.text(hex2a(window.atob(szparam.CardInfo.Name)))
          sex.text(hex2a(window.atob(szparam.CardInfo.Sex)))
          nation.text(hex2a(window.atob(szparam.CardInfo.Nation)))
  
          const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
          const birthArr = parseDateString(Birthday , ".", true).split(".")
          year.text(birthArr[0])
          month.text(birthArr[1])
          date.text(birthArr[2])
  
          address.text(hex2a(window.atob(szparam.CardInfo.Address)))
          number.text(hex2a(window.atob(szparam.CardInfo.No)))
          
          department.text(hex2a(window.atob(szparam.CardInfo.SignedDepartment)))
  
          const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
          const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
          const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
          const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
          expiry.text( expiryBegin + '-' + expiryEnd)
  
          image.attr('src','data:image/jpg;base64,' + szparam.BmpInfo)

        } else if (jsonobject.Cmd == 30401) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读取A卡SN成功：' + szparam.SN)
        } else if (jsonobject.Cmd == 20401) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读取身份证卡片SN成功：' + szparam.SN)
        } else if (jsonobject.Cmd == 20511) {
          const szparam = JSON.parse(window.atob(jsonobject.UserParam))
          processContent.text('websocket 协议 读卡器唯一号：' + szparam.SN)
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
    name.text('')
    sex.text('')
    nation.text('')
    year.text('')
    month.text('')
    date.text('')
    address.text('')
    number.text('')
    department.text('')
    expiry.text('')
    image.attr('src', '')
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

        SNContent.text(szparam.CardInfo.SN)
          name.text(hex2a(window.atob(szparam.CardInfo.Name)))
          sex.text(hex2a(window.atob(szparam.CardInfo.Sex)))
          nation.text(hex2a(window.atob(szparam.CardInfo.Nation)))
  
          const Birthday = hex2a(window.atob(szparam.CardInfo.Birthday))
          const birthArr = parseDateString(Birthday , ".", true).split(".")
          year.text(birthArr[0])
          month.text(birthArr[1])
          date.text(birthArr[2])
  
          address.text(hex2a(window.atob(szparam.CardInfo.Address)))
          number.text(hex2a(window.atob(szparam.CardInfo.No)))
          
          department.text(hex2a(window.atob(szparam.CardInfo.SignedDepartment)))
  
          const ValidityPeriodBegin = hex2a(window.atob(szparam.CardInfo.ValidityPeriodBegin))
          const ValidityPeriodEnd = hex2a(window.atob(szparam.CardInfo.ValidityPeriodEnd)).trim()
          const expiryBegin = parseDateString(ValidityPeriodBegin, '.')
          const expiryEnd = ValidityPeriodEnd !== '长期' ? parseDateString(ValidityPeriodEnd, '.') : ValidityPeriodEnd
          expiry.text( expiryBegin + '-' + expiryEnd)
  
          image.attr('src','data:image/jpg;base64,' + szparam.BmpInfo)

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