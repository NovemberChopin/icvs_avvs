#ifndef __AVOS_UTIL_H__
#define __AVOS_UTIL_H__


class AvosUtil
{
public:
	AvosUtil();
	~AvosUtil();


//execute shell command
//执行一个shell命令，输出结果逐行存储在resvec中，并返回行数
	int ExecShell(const char *cmd, std::vector<std::string> &resvec) {
		resvec.clear();
		FILE *pp = popen(cmd, "r"); //建立管道
		if (!pp) {
			return -1;
		}
		char tmp[1024]; //设置一个合适的长度，以存储每一行输出
		while (fgets(tmp, sizeof(tmp), pp) != NULL) {
			if (tmp[strlen(tmp) - 1] == '\n') {
				tmp[strlen(tmp) - 1] = '\0'; //去除换行符
			}
			resvec.push_back(tmp);
		}
		pclose(pp); //关闭管道
		return resvec.size();
	}


	static bool getPackagePath(std::string package_name, std::string &res) {
		std::vector<std::string> resvec;
		std::string cmd = "rospack find " + package_name;
		ExecShell(cmd.c_str(), resvec);
		if (resvec.size() == 0)
			return false;
		res = resvec[0];
		return true;
	}



};



#endif