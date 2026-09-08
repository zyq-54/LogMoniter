
import time
from datetime import datetime,timedelta
import os
import requests
import json
import os

# --- 配置区 ---
API_KEY = "sk-9018ca4e889f436693e9ea89056e087c"  # 粘贴你刚才复制的 sk- 开头的密钥
LOG_FILE = "output.txt"
def read_recent_lines(file_path,max_bytes=4096):
    if not os.path.exists(file_path):
        print("日志文件不存在")
        return[]
    try:
        with open(file_path,'r',encoding='utf-8', errors='ignore')as f:
            f.seek(0,os.SEEK_END)
            size=f.tell()
            if size == 0:
                return[]
            if size>max_bytes:
                f.seek(size-max_bytes,os.SEEK_SET)
            else:
                f.seek(0,os.SEEK_SET)
            data = f.read()
            lines = data.splitlines()
            return lines
    except FileNotFoundError:
        print("文件在检查后被移除")
        return[]
    except Exception as e:
        print(f"[ERROR]读取文件出错：{e}")
        return[]

def ask_ai(log_file=LOG_FILE):
    recent_lines = read_recent_lines(LOG_FILE, 5)
    if not recent_lines:
        print("[INFO] 日志文件为空或不存在，无法分析。")
        return

    # 2. 构造发送给 AI 的提问内容
    log_text = "".join(recent_lines)
    user_prompt = f"请分析以下错误日志，并给出可能的原因和解决建议：\n\n{log_text}"

    # 3. 准备 HTTP 请求
    url = "https://api.deepseek.com/chat/completions"
    headers = {
        "Authorization": f"Bearer {API_KEY}",
        "Content-Type": "application/json"
    }
    payload = {
        "model": "deepseek-chat",
        "messages": [
            {"role": "system", "content": "你是一个精通 C++ 和网络编程的系统工程师，善于分析日志。"},
            {"role": "user", "content": user_prompt}
        ],
        "stream": False
    }

    # 4. 发送请求并打印 AI 回答
    try:
        response = requests.post(url, headers=headers, json=payload)
        if response.status_code == 200:
            result = response.json()
            ai_reply = result['choices'][0]['message']['content']
            print("\n" + "=" * 50)
            print("🤖 AI 分析结果：")
            print(ai_reply)
            print("=" * 50)
        else:
            print(f"[ERROR] 请求失败，状态码：{response.status_code}")
            print(f"错误信息：{response.text}")
    except Exception as e:
        print(f"[ERROR] 发生异常：{e}")
if __name__ == '__main__':
    last_alert_time = 0
    while True:
        print("[DEBUG] 监控循环运行中...")
        lines = read_recent_lines("output.txt")
        now = datetime.now()
        error_count = 0
        for line in lines:
            if len(line)<19:
                continue
            time_str = line[:19]
            try:
                log_time = datetime.strptime(time_str,"%Y-%m-%d %H:%M:%S")
            except  ValueError:
                continue
            if(now-log_time).total_seconds()<=60:
                if"[ERROR]"in line:
                    error_count += 1
        print(f"[DEBUG] 当前错误数: {error_count}")
        if error_count>5 and(time.time()-last_alert_time)>30:
            print("\033[91m[ALERT] 过去 60 秒内错误超过 5 条！\033[0m")
            ask_ai(log_file=LOG_FILE)
            last_alert_time = time.time()
        time.sleep(5)
