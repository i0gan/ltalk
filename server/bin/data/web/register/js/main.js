// Author: I0gan

var btn = document.getElementById("button_rigister");
var server_url = 'http://192.168.100.4';
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

    if(name.length == 0) {
        alert("姓名为空")
        return;
    }
    
    if(email.length == 0) {
        alert("邮件为空")
        return;
    }
    
    if(password_1.length < 6) {
        alert("密码必须 6~32位");
        return;
    }
    
    if(password_1 != password_2) {
        alert("密码不一致");
        return;
    }
    xhr = new XMLHttpRequest();
    // 开始发送
    xhr.open('post', server_url);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.onreadystatechange = function() {
        if(xhr.readyState == 4) {
            reply(xhr);
           
           
        }
    }
    var data = new Date();
    var json = {
        request:"register", platform: "web", token:"none"
    };
    json.content = {};
    json.content.name = name;
    json.content.email = email;
    json.content.phone_number = phone_number;
    json.content.address = address;
    json.content.occupation = address;
    json.content.password_2 = password_2;
    
    //json.cmd = "aa";
    json_str = JSON.stringify(json);
    //var json = {"request":"register", "data", date.toLocaleString(); };
    xhr.send(json_str);
    alert("OK");
}


function reply(xhr) {
    alert(xhr.responseText);
    
    
}
