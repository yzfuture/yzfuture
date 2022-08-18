
import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

public class TwoCardByteArray extends Structure {
    @Override protected List getFieldOrder() {
        return Arrays.asList(new String[]{"arrTwoIdName",
                "arrTwoIdSex",
                "arrTwoIdNation",
                "arrTwoIdBirthday",
                "arrTwoIdAddress",
                "arrTwoIdNo",
                "arrTwoIdSignedDepartment",
                "arrTwoIdValidityPeriodBegin",
                "arrTwoIdValidityPeriodEnd",
                "arrTwoOtherNO",
                "arrTwoSignNum",
                "arrTwoRemark1",
                "arrTwoType",
                "arrTwoRemark2",
                "arrTwoIdNewAddress",
                "arrReserve",
                "arrTwoIdPhoto",
                "arrTwoIdFingerprint",
                "arrTwoIdPhotoJpeg",
                "unTwoIdPhotoJpegLength"});
    }

    /**
     * 姓名 UNICODE
     */
    public byte[] arrTwoIdName = new byte[30];
    /**
     * 性别 UNICODE
     */
    public byte[] arrTwoIdSex = new byte[2];
    /**
     * 民族 UNICODE
     */
    public byte[] arrTwoIdNation = new byte[4];
    /**
     * 出生日期 UNICODE YYYYMMDD
     */
    public byte[] arrTwoIdBirthday = new byte[16];
    /**
     * 住址 UNICODE
     */
    public byte[] arrTwoIdAddress = new byte[70];
    /**
     * 身份证号码 UNICODE
     */
    public byte[] arrTwoIdNo = new byte[36];
    /**
     * 签发机关 UNICODE
     */
    public byte[] arrTwoIdSignedDepartment = new byte[30];
    /**
     * 有效期起始日期 UNICODE YYYYMMDD
     */
    public byte[] arrTwoIdValidityPeriodBegin = new byte[16];
    /**
     * 有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
     */
    public byte[] arrTwoIdValidityPeriodEnd = new byte[16];

    public byte[]  arrTwoOtherNO = new byte[18]; // 通行证类号码
    public byte[]  arrTwoSignNum = new byte[4];	// 签发次数
    public byte[]  arrTwoRemark1 = new byte[6];	// 预留区
    public byte[]  arrTwoType = new byte[2];		// 证件类型标识
    public byte[]  arrTwoRemark2 = new byte[6];	// 预留区

    /**
     * 最新住址 UNICODE
     */
    public byte[] arrTwoIdNewAddress = new byte[70];
    public byte[] arrReserve = new byte[2];
    /**
     * 照片信息
     */
    public byte[] arrTwoIdPhoto = new byte[1024];
    /**
     * 指纹信息
     */
    public byte[] arrTwoIdFingerprint = new byte[1024];

    public byte[] arrTwoIdPhotoJpeg = new byte[4096];
    public int unTwoIdPhotoJpegLength;
}
