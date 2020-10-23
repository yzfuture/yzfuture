unit duType;

interface

uses
  Winapi.Messages, Vcl.Graphics, Winapi.Windows, System.SysUtils;

type
  TMSG_ID = (//
  ///<summary>日志消息</summary>
    TMSG_ID_SendStr = WM_USER + 1024, // 收到日志消息

    TMSG_ID_ReadIDCard  //身份证读卡消息
);


  TMSG_wParam = (//

  ///<summary>读卡进度条</summary>
    TMSG_wParam_ReaderProgress,
    ///<summary>读身份证状态</summary>
    TMSG_wParam_ReadIDCard

);




type
  // 要传递的消息记录.
  TStr_MSG = record
    MSG: array[0..1023] of AnsiChar; // 打印的文字
  end;

  pStr_MSG = ^TStr_MSG;

type
  TParam = record
    Path: string;

  end;

implementation

uses
  duGlob;

initialization
  G_Param.Path := ExtractFilePath(ParamStr(0));

finalization

end.

