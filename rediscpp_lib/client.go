//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

//未测试

//client.go - A client for the Rediscpp daemon.

package Rediscpp

import(
	"fmt"
	"net"
	"bytes"
	"io/ioutil" //io 工具包	
)

type redisError struct {  
    Err    error
}

func (e *redisError) Error() string {  

    return e.Err.Error()
}

//禁止直接生成rediscpp
type rediscpp struct{
	var server string
	var port  int
	var addr Addr
	var conn *TCPConn
}

//构造函数
func New(server string, port int) (rediscpp, error) {  
    c := rediscpp {server, port}

    c.addr, err := net.ResolveTCPAddr("tcp4", c.server)
	if err != nil{
		fmt.Println(err)
		return
	}

	//func DialTCP(net string, laddr, raddr *TCPAddr) (*TCPConn, error) 
	c.conn, err := net.DialTCP("tcp4", nil, c.addr)
	if err != nil{
		fmt.Println(err)
		return
	}
	
    return c
}

//func (c rediscpp)connect(){
//	addr, err := net.ResolveTCPAddr("tcp4", c.server)
//	if err != nil{
//	}
//	
//}

//func disconnect() bool{
//	defer func
//}

//func write(buffer bytes.Buffer){
//}

//func read(){
//}

func Get(key string)([]byte){
	//var buffer bytes.Buffer
	buffer := bytes.NewBufferString("get ") 
	b.WriteString(key)
	_, err = conn.WriteString(buffer)
	//func ReadAll(r io.Reader) ([]byte, error)
	response, _ := ioutil.ReadAll(conn)
	return response
}

func Set(key string, value string){
	buffer := bytes.NewBufferString("set ") 
	b.WriteString(key + value)
	_, err = conn.WriteString(buffer)
	//func ReadAll(r io.Reader) ([]byte, error)
	response, _ := ioutil.ReadAll(conn)
	return response
}