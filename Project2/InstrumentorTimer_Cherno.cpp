#include<iostream>
//#include<memory>
#include<string>
#include<fstream>
#include<chrono>
#include<algorithm>
#include<thread>
#include<cmath>

struct ProfileResult
{
	std::string Name;
	long long Start, End;
	uint64_t ThreadID;
};

struct InstrumentationSession
{
	std::string Name;
};

class Instrumentor
{
private:
	InstrumentationSession* m_CurrentSession;
	std::string m_SessionName;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
public:
	Instrumentor()
		: m_CurrentSession(nullptr), m_ProfileCount(0)
	{

	}

	void BeginSession(const std::string& name, const std::string& filepath = "result.json")
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}
	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();

		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;

	}

	void WriteProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";
		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"fuction\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadID << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}
	void WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	static Instrumentor& Get()
	{
		static Instrumentor* instance = new Instrumentor();
		return *instance;
	}

};



class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}
	
	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		uint64_t ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		std::cout << m_Name << ": " << (end - start) << "ms\n";
		Instrumentor::Get().WriteProfile({ m_Name, start, end, ThreadID });
		//作用域结束时自动记录时间并将结果写入性能分析器。
		m_Stopped = true;
	}


private:

	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	

};

#define PROFILING 1
#if PROFILING
	#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PROFILE_SCOPE(name)
#endif

namespace RUN {
	void f1(int value)
	{
		PROFILE_FUNCTION();

		for (int i = 0; i < 10000; i++) {
			std::cout << "hell0 #" << (i + value) << std::endl;
		}
	}
	void f1()
	{
		PROFILE_FUNCTION();

		for (int i = 0; i < 10000; i++) {
			std::cout << "hell0 world #" << i << std::endl;
		}
	}

	void run()
	{
		PROFILE_FUNCTION();

		std::cout << "jiojo" << std::endl;
		std::thread a([]() {f1(3); });
		std::thread b([]() {f1(); });
		f1(3);
		f1();
		a.join();
		b.join();
	}
}


int main()
{
	Instrumentor::Get().BeginSession("Profile");
	RUN::run();
	Instrumentor::Get().EndSession();

	std::cin.get();
}