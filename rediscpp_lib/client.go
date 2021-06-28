//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

//有点小问题

//client.go - A client for the Rediscpp daemon.

package rediscpp

import(
    "fmt"
    "net"
    "strconv"
    "os"
    "bytes"
    "io/ioutil" //io 工具包
)

//禁止直接生成rediscpp
type rediscpp struct{
    server string
    port  int
    conn net.Conn
}

//构造函数
func Newcpp(server string, port int) (rediscpp, error) {
    //c := rediscpp {server, port, net.Conn{}}
    var c rediscpp
    c.server = server
    c.port = port

    conn, err := net.Dial("tcp", server + ":" + strconv.Itoa(port))
    c.conn = conn
    
    if err != nil{
        fmt.Println(err)
    }

    return c, err
}

func (c rediscpp)Get(key string)([]byte){
        //var buffer bytes.Buffer
        buffer := bytes.NewBufferString("get ")
        _, _ = buffer.WriteString(key) //暂时忽略返回
        _, _ = c.conn.Write(buffer.Bytes())
        //func ReadAll(r io.Reader) ([]byte, error)
        response, _ := ioutil.ReadAll(c.conn)
        return response
}

func (c rediscpp)Set(key string, value string)([]byte){
        buffer := bytes.NewBufferString("set ")
        _, _ = buffer.WriteString(key + " " + value)
        _, _ = c.conn.Write(buffer.Bytes())
        //func ReadAll(r io.Reader) ([]byte, error)
        response, _ := ioutil.ReadAll(c.conn)
        return response
}

/*
func main(){
    ip, port := "127.0.0.1", 9898
    
    client, err := Newcpp(ip, port)
    
     ClientHandleError(err, "error")
    
    var res []byte
    
    res = client.Set("hello", " world")
    fmt.Println(res)
    res = client.Get("hello")
    fmt.Println(res)
}
*/

func ClientHandleError(err error, what string){
    if err != nil {
        fmt.Println(err, what)
        os.Exit(1)
    }
}