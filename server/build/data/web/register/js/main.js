// Author: I0gan

var btn = document.getElementById("button_rigister");
//var server_url = 'http://192.168.100.4/?request=register&platform=web';
btn.onclick = function() {
    
    var name = document.getElementById("name").value;
    var email = document.getElementById("email").value;
    var phone_number = document.getElementById("phone_number").value;
    var address = document.getElementById("address").value;
    var select_obj = document.getElementById("occupation");
    var select_index = select_obj.selectedIndex;
    var occupation = select_obj[select_index].value;
    var password_1 = document.getElementById("password_1").value;
    var password_2 = document.getElementById("password_2").value;

    if(name.length === 0) {
        alert("姓名为空")
        return;
    }
    
    if(email.length === 0) {
        alert("邮件为空")
        return;
    }
    
    if(password_1.length < 6 || password_1.length > 32) {
        alert("密码必须 6~32位");
        return;
    }
    
    if(password_1 !== password_2) {
        alert("密码不一致");
        return;
    }
    xhr = new XMLHttpRequest();
    // 开始发送
    xhr.open('post', window.location.href);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.onreadystatechange = function() {
        if(xhr.readyState === 4) {
            reply(xhr);
        }
    }
    var json = {
        request: 0, token:"none", uid:"none", content_type: "register_info"
    };

    json.datetime = nowDateTime();
    json.content = {};
    json.content.name = name;
    json.content.email = email;
    json.content.phone_number = phone_number;
    json.content.address = address;
    json.content.occupation = occupation;
    json.content.password = window.btoa(password_2);
    json_str = JSON.stringify(json);
    //alert(json_str)
    xhr.send(json_str);
}

function reply(xhr) {
    //alert('收到');
    var json = JSON.parse(xhr.responseText);
    var code = json.code;
    var server = json.server;
    var access_url = json.access_url;
    var datetime = json.datetime;
    var token = json.token;
    
    //alert(xhr.responseText);
    if(code == "0") {
        alert("注册成功!");
        window.location.href = access_url;
    }else if(code == "11"){
        alert("帐号已经存在");
    }else {
        alert("注册失败");
    }
}

// 将ajax 返回的时间戳 转为 “yyyy-MM-dd” 型
function nowDateTime(){
    var now = new Date();
    var year = now.getFullYear();
    var month =(now.getMonth() + 1).toString();
    var day = (now.getDate()).toString();
    var hour = now.getHours().toString();
    var minute = now.getMinutes().toString();
    var second = now.getSeconds().toString();
    if (month.length === 1) {
        month = "0" + month;
    }
    if (day.length === 1) {
        day = "0" + day;
    }
    if (hour.length === 1) {
        hour = "0" + hour;
    }
    var dateTime = year +"-"+ month +"-"+  day + " " + hour + ":" + minute + ":" + second;
    return dateTime;
}
