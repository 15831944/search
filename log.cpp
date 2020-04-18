#include "log.h"


CGLog * CGLog::m_pInstance = nullptr;
CGLog::CGLog(void)
{
#ifdef LOG_OUT_PRINT
	//memset(pLogBuff, 0, MAX_BUFF_LOG_LEN);
#endif
}

CGLog::~CGLog(void)
{
#ifdef LOG_OUT_PRINT
	
	google::ShutdownGoogleLogging();
#endif
}

int32_t CGLog::InitGLog(const char * argv0)
{	
#ifdef LOG_OUT_PRINT
	mkdir("./log", 0755);
	google::InitGoogleLogging(argv0);

	google::SetStderrLogging(google::GLOG_INFO); //���ü������ google::INFO ����־ͬʱ�������Ļ
	google::SetLogDestination(google::GLOG_FATAL, "./log/log_fatal_"); // ���� google::FATAL �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::GLOG_ERROR, "./log/log_error_"); //���� google::ERROR �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::GLOG_WARNING, "./log/log_warning_"); //���� google::WARNING �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::GLOG_INFO, "./log/log_info_"); //���� google::INFO �������־�洢·�����ļ���
	
	FLAGS_max_log_size = 100; //�����־��СΪ 100MB
	FLAGS_stop_logging_if_full_disk = true; //�����̱�д��ʱ��ֹͣ��־���
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true; //�����������Ļ����־��ʾ��Ӧ��ɫ
	//FLAGS_log_prefix = true;  //������־ǰ׺�Ƿ�Ӧ����ӵ�ÿ�����
	//FLAGS_stop_logging_if_full_disk = true;  //�����Ƿ��ڴ�������ʱ������־��¼������
#endif	
	return 0;
}

void CGLog::GLogMsg(const char * funName, int lineNum, uint32_t nLogSeverity, const char *format, ...)
{
#ifdef LOG_OUT_PRINT
	string strTemp;
	strTemp.resize(MAX_BUFF_LOG_LEN);
	char *pLogBuff = const_cast<char*>(strTemp.data());
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(pLogBuff,MAX_BUFF_LOG_LEN, format, arg_ptr);
	switch (nLogSeverity)
	{
	case 0:
		LOG(INFO) << "[" << funName << ":" << lineNum <<"] " << pLogBuff;
		break;
	case 1:
		LOG(WARNING) << "[" << funName << ":" << lineNum << "] " << pLogBuff;
		break;
	case 2:
		LOG(ERROR) << "[" << funName << ":" << lineNum << "] " << pLogBuff;
		break;
	case 3:
		LOG(FATAL) << "[" << funName << ":" << lineNum << "] " << pLogBuff;
		break;
	default:
		break;
	}
	va_end(arg_ptr);

	//memset(pLogBuff, 0, MAX_BUFF_LOG_LEN);
	
#endif
	return;
}

