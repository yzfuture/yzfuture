package com.android.readtwogeneralcard.myapplication;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RegisterUser {

    public class RetParam
    {
        public String   szError;
        public int      nRetCode;
        public String   szUser;
        public String   szPwd;
        public ArrayList<Map<String, String>> appArray = null;
    }

    public RetParam registrUser(String szphone, String szcode)
    {
        RetParam            retInfo = null;
        JSONObject          tableObj = new JSONObject();
        JSONObject          paramObj = new JSONObject();
        try {
            tableObj.put("cmd", 113);
            paramObj.put("phone", szphone);
            paramObj.put("verCode", szcode);
            tableObj.put("userParam", paramObj);
            Map<String, String>  szrecode = onPost(tableObj.toString(), "https://id.yzfuture.cn/api/v2/ipmanagement/demo-apply-trial");
            if (Integer.parseInt(szrecode.get("code")) == 200)
            {
                retInfo     = new RetParam();
                JSONObject jsonObject=new JSONObject(szrecode.get("recode"));
                int         nerror = jsonObject.optInt("retCode", 0);
                retInfo.nRetCode = nerror;
                retInfo.szError = jsonObject.optString("errorData", "");
                if (nerror == 1)
                {
                    JSONObject  retparamObj = jsonObject.getJSONObject("userParam");
                    retInfo.szUser = retparamObj.optString("account");
                    retInfo.szPwd = retparamObj.optString("pasd");

                    JSONArray appArray = retparamObj.getJSONArray("apps");
                    retInfo.appArray = new ArrayList<Map<String, String>>();
                    for (int i = 0; i < appArray.length(); i++) {
                        JSONObject appObj = appArray.getJSONObject(i);
                        Map<String, String> appMap = new HashMap<String, String>();
                        appMap.put("name", appObj.optString("name", ""));
                        appMap.put("appKey", appObj.optString("appKey", ""));
                        appMap.put("appSecret", appObj.optString("appSecret", ""));
                        retInfo.appArray.add(appMap);
                    }
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }catch (IOException e) {
            e.printStackTrace();
        }
        return  retInfo;
    }
    public boolean getCode(String szphone)
    {
        JSONObject          tableObj = new JSONObject();
        JSONObject          paramObj = new JSONObject();
        try {
            tableObj.put("cmd", 111);
            paramObj.put("phone", szphone);
            paramObj.put("type", 2);
            tableObj.put("userParam", paramObj);
            Map<String, String>  szrecode = onPost(tableObj.toString(), "https://id.yzfuture.cn/api/v2/ipmanagement/demo-vercode");
            if (Integer.parseInt(szrecode.get("code")) == 200)
            {
                String      szret = szrecode.get("recode");
                JSONObject retObj = new JSONObject(szret);
                if (retObj.optInt("retCode", 0) == 1)
                {
                    return true;
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }catch (IOException e) {
            e.printStackTrace();
        }
        return  false;
    }

    private Map<String, String> onPost(String szparam, String szurl) throws IOException, JSONException {
        URL                 url = new URL(szurl);
        HttpURLConnection   conn = (HttpURLConnection)url.openConnection();
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Accept", "application/json");
        conn.setRequestProperty("Content-Type", "application/json");
        conn.setDoOutput(true);

        OutputStream        os = conn.getOutputStream();
        os.write(szparam.getBytes("utf-8"));
        os.flush();

        InputStream         is = conn.getInputStream();
        BufferedReader      reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder       sb = new StringBuilder();
        String line = null;
        while((line=reader.readLine()) != null)
        {
            sb.append(line);
        }

        Map<String, String> map = new HashMap<String, String>();
        map.put("code", String.valueOf(conn.getResponseCode()));
        map.put("recode", sb.toString());
        conn.disconnect();
        return map;
    }
}
