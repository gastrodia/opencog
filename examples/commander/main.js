
const socket = new WebSocketIO({port: 8000})

const vm = new Vue({
    el: "#app",
    data: {
        text_content: "hello, this is a message box",
        input_text: "type some text"
    },
    methods: {
        submit() {
            socket.send("message", this.input_text)
            this.input_text = ''
        }
    }
})

var methods = {
    foo:function(str){
        return str + ' world!'
    }
}

socket.on("connection", () => {


    socket.send('psi-run!',{});


    socket.on("rpc-execute!",(data)=>{
         //{"method":"method_name","params":[xx,123,xxx]}
        var result = methods[data.method].apply(this,data.params)
        data["result"] = result;
        socket.send('rpc-result!',data)

        vm.text_content += `\n psi-run! call client success.`
    })

    
})