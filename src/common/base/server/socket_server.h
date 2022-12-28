#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "common/base/thread_pool/include/thread_pool.h"
#include "common/base/macros.h"
#include "rosbridge.h"
#include "data_pool.h"

#define BUFFER_SIZE 100
#define MAX_CLIENT_NUM 10
#define THREAD_NUM 	15

namespace avos {
namespace base {


typedef struct ServerHeader
{
	unsigned char magic_num_1;
	unsigned char magic_num_2;
	char type;      // 1 heartbeat   2 mode content
	char executie_mode;
	char task_recv_mode;
	char task_send_mode;
	char task_setting_mode;
	bool IsServerProtocol() {
		if (magic_num_1 != 0x55 || magic_num_2 != 0xcc) {
			return false;
		}
		return true;
	}
	ServerHeader() {
		magic_num_1 = 0x55;
		magic_num_2 = 0xcc;
		type = 0;
		task_recv_mode = -1;
		task_send_mode = -1;
		executie_mode = -1;
		task_setting_mode = -1;
	}

} ServerHeader;

class SocketServer
{
public:
	SocketServer() {
		server_ip = "";
		server_port = -1;
		sockfd = -1;
		new_fd = -1;
		client_counter = 0;
	}
	~SocketServer() {

	}

	bool InitServer(const std::string& _server_ip, const  int& _server_port) {
		server_ip = _server_ip;
		server_port = _server_port;
		my_addr.sin_family = AF_INET; //IPV4
		my_addr.sin_port = htons(server_port); //绑定端口9999，并将其转化为网络字节
		my_addr.sin_addr.s_addr = INADDR_ANY; //指定接收IP为任意（如果指定IP，则只接收指定IP）
		bzero(&(my_addr.sin_zero), 0); //最后位置赋0，补位置
//设置socked
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket error!\n");
			return false;
		}
		int sockflag = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockflag, sizeof(int)) < 0) {
			perror("setsockopt error!\n");
			return false;
		}
//绑定socked
		if ((bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr))) < 0)
		{
			perror("bind error!\n");
			return false;
		}
//开启监听,可连接客户端最大为10个
		if (listen(sockfd, MAX_CLIENT_NUM) == -1)
		{
			perror("listen error!\n");
			return false;
		}

		return true;
	}

	int WaitConnection() {
		if (server_port <= 0) {
			perror("server is uninit!\n");
			return -1;
		}
		auto thread_pool = ThreadPool::Instance();
		while (avos::task::Rosbridge::OK())
		{
			int sin_size = sizeof(struct sockaddr_in);
//等待客户端连接，连接后their_addr接收客户端的IP等信息，没有客户端连接，则一直等待
			if ((new_fd = accept(sockfd,  (struct sockaddr*)(&their_addr), (socklen_t*)&sin_size)) < 0)
			{
				perror("accept error!\n");
				return 0;
			}
//连接成功后，连接客户端数+1
			client_counter++;
			for (int i = 0; i < THREAD_NUM; i++) {
				if (thread_pool->CheckThreadState(i) == eThreadStatus::WAITING) {
					thread_pool->ResetThreadStatus(i);
				}
			}
//开启进程运行客户端
			int thread_handle = thread_pool->getIdleThreadHandle();
			const auto work = [&]() {
				ListenData(new_fd, client_counter);
				return 0;
			};
			thread_pool->AddThreadWork(thread_handle, work);

		}
		close(sockfd);
		printf("server-------closed.\n");
		return 0;
	}

private:
	void ListenData(int fd, int id)
	{
		auto DP = avos::task::DataPool::Instance()->GetMainDataPtr();
		char buffer[BUFFER_SIZE];
		while (1) //一直处于监听客户端信息状态,知道客户端断开或客户端发出退出信息
		{
			memset(buffer, 0, BUFFER_SIZE);
			int len = recv(fd, buffer, BUFFER_SIZE, 0);
			if (len > 0) //如果接受到数据，则判断是否为退出信息
			{
				// printf("ID_%d:%d\n", id, len); //输出第N 个用户，输出的信息
				ServerHeader *pServerHeader = (ServerHeader *)buffer;
				if (pServerHeader->IsServerProtocol() == false) continue;
				if (pServerHeader->type == 0x01) {      //heart beat
				} else if (pServerHeader->type == 0x03) {
					std::cout << "[taskcenter] rev task_recv_mode:" << (int)pServerHeader->task_setting_mode << std::endl;
					DP->task_recv_mode = pServerHeader->task_setting_mode;
					pServerHeader->type  = 0x02;
					pServerHeader->task_recv_mode =  pServerHeader->task_setting_mode;
				} else {

				}
				send(fd, (char *)pServerHeader, sizeof(ServerHeader), 0);
			}
			else //接收数据小于0，或等于0 则退出
			{
				printf("clinet %d close!\n", id);
				break;
			}
//如果服务端需要发送信息，此处添加发送信息
//memset(buffer,0,BUFFER_SIZE);
//scanf("%s",buffer);
//send(fd,buffer,BUFFER_SIZE,0);
		}
		close(fd); //关闭此客户端的连接的socket
	}
private:
	std::string server_ip;
	int server_port;
	int sockfd, new_fd;

	struct sockaddr_in my_addr; //本地连接socked
	struct sockaddr_in their_addr; //客户端连接socked
	char buffer[BUFFER_SIZE];
	int client_counter;  //记录连接客户端数目，可以使用数组，结构体等数据类型记录客户端信息（IP，端口等）
//记录本地信息

};
}
}












#endif
