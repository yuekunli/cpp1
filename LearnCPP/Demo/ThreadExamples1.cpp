#include "../LibHeaders.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>

namespace ThreadExamples1 {

	namespace Thread_Construct {

		void f1(int n)
		{
			for (int i = 0; i < 5; i++)
			{
				std::cout << "Thread 1 executing\n";
				n++;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		void f2(int& n)
		{
			for (int i = 0; i < 5; i++)
			{
				std::cout << "Thread 2 executing\n";
				n++;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		class foo
		{
		public:
			void bar()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Thread 3 executing\n";
					n++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
			int n = 0;
		};

		class baz
		{
		public:
			void operator()()
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Thread 4 execting\n";
					n++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
			int n = 0;
		};

		void Test_Thread_Construct()
		{
			int n = 0;
			foo f;
			baz b;
			std::thread t1;
			std::thread t2(f1, n + 1);
			std::thread t3(f2, std::ref(n));
			std::thread t4(std::move(t3));
			std::thread t5(&foo::bar, &f);
			std::thread t6(b);
			std::thread t7(std::ref(b));
			t2.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			std::cout << "Final value of n is " << n << '\n';
			std::cout << "Final value of f.n (foo::n) is " << f.n << '\n';
			std::cout << "Final value of b.n (baz::n) is " << b.n << '\n';
		}
	}

	namespace Thread_Joinable {
		void foo()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		void Test_Thread_Joinable()
		{
			std::thread t;
			std::cout << "before start, joinable: " << std::boolalpha << t.joinable() << '\n';
			t = std::thread(foo);
			std::cout << "after starting, joinable: " << t.joinable() << '\n';
			t.join();
			std::cout << "after joining, joinable: " << t.joinable() << '\n';
		}
	}

	namespace Thread_Detach {
		static void independentThread()
		{
			std::cout << "Starting concurrent thread.\n";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::cout << "Exiting concurrent thread.\n";
		}
		static void threadCaller()
		{
			std::cout << "Starting thread caller.\n";
			std::thread t(independentThread);
			t.detach();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "Exiting thread caller.\n";
		}
		void Test_Thread_Detach()
		{
			threadCaller();
			std::this_thread::sleep_for(std::chrono::seconds(4));
			std::cout << "Exiting main thread.\n";
		}
	}

	namespace Thread_Swap {
		static void foo()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		static void bar()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		void Test_Thread_Swap()
		{
			std::thread t1(foo);
			std::thread t2(bar);

			std::cout << "Thread 1 id: " << t1.get_id() << '\n'
				<< "Thread 2 id: " << t2.get_id() << '\n';


			std::swap(t1, t2);

			std::cout << "\n\nThread 1 id: " << t1.get_id() << '\n'
				<< "Thread 2 id: " << t2.get_id() << '\n';


			t1.swap(t2);

			std::cout << "\n\nThread 1 id: " << t1.get_id() << '\n'
				<< "Thread 2 id: " << t2.get_id() << '\n';

			t1.join();
			t2.join();
		}
	}

	namespace ConditionVariable {
		std::mutex m;
		std::condition_variable cv;
		std::string data;
		bool ready = false;
		bool processed = false;

		static void worker_thread()
		{
			std::unique_lock<std::mutex> lk{m};
			cv.wait(lk, []() {return ready; });
			std::cout << "worker thread is processing data\n";
			data += " after processing";
			processed = true;
			std::cout << "Worker thread signals data processing completed\n";
			lk.unlock();
			cv.notify_one();
		}

		void Test_ConditionVariable()
		{
			std::thread worker(worker_thread);
			data = "Example data";

			{
				std::lock_guard<std::mutex> lk(m);
				ready = true;
				std::cout << "main() signals data ready for processing\n";
			}

			cv.notify_one();

			{
				std::unique_lock<std::mutex> lk(m);
				cv.wait(lk, [] {return processed; });
			}

			std::cout << "Back in main(), data = " << data << '\n';

			worker.join();
		}
	}

	namespace Lock {
		struct Employee
		{
			Employee(std::string id) :id(id)
			{
			}
			std::string id;
			std::vector<std::string> lunch_partners;
			std::mutex m;
			std::string output() const
			{
				std::string ret = "Employee " + id + " has lunch partners: ";
				for (auto const& partner : lunch_partners)
					ret += partner + " ";
				return ret;
			}
		};

		static void send_mail(Employee&, Employee&)
		{
			// simulate sending mail
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		static void assign_lunch_partner(Employee& e1, Employee& e2)
		{
			static std::mutex io_mutex;
			{
				std::lock_guard<std::mutex> lk{ io_mutex };
				std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
			}
			{
				std::lock(e1.m, e2.m);
				std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
				std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);
				{
					std::lock_guard<std::mutex>lk{ io_mutex };
					std::cout << e1.id << " and " << e2.id << " got locks " << std::endl;
				}
				e1.lunch_partners.push_back(e2.id);
				e2.lunch_partners.push_back(e1.id);
			}
			send_mail(e1,e2);
			send_mail(e2, e1);
		}

		void Test_Lock()
		{
			Employee alice("alice"), bob("bob"), christina{ "christina" }, dave("dave");

			std::vector<std::thread> threads;
			threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
			threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
			threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
			threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

			for (auto& t : threads)
				t.join();

			std::cout << alice.output() << '\n' << bob.output() << '\n' << christina.output() << '\n' << dave.output() << '\n';
		}
	}


	namespace Pipe {
		using namespace std;

		enum DataAndThreadState
		{
			readyForInput = 0,
			dataAvailable = 1
		};

		struct Stage
		{
			mutex m;
			condition_variable dataAvailable;
			condition_variable readyForInput;
			DataAndThreadState state;
			int data;
			thread t;
			Stage* next;
			bool requestTerminate{ false };
		};

		struct Pipe
		{
			mutex m;
			Stage* head;
			Stage* tail;
			int stageCount;
			int dataCount;
		};

		int sendToStage(Stage& sender, Stage& receiver, int value, bool isSenderMain = false)
		{
			/*
			* Note that if stage n's thread is stuck in this function, that means stage n's thread is trying to
			* send data to stage n+1. The input argument 's' actually represent stage n+1
			*/
			unique_lock<mutex> ulk(receiver.m, defer_lock);
			bool result = ulk.try_lock();
			if (!result && isSenderMain)
				return 2;
			else if (!result)
				ulk.lock();
			receiver.readyForInput.wait(ulk, [&]() {return receiver.state == readyForInput || sender.requestTerminate; });
			if (sender.requestTerminate)
				return 3;
			receiver.data = value;
			receiver.state=dataAvailable;
			receiver.dataAvailable.notify_one();
			return 1;
		}

		void stageDemon(Stage* s)
		{
			Stage& nextStage = *(s->next);
			{
				unique_lock<mutex> ulk(s->m);
				while (true)
				{
					s->dataAvailable.wait(ulk, [&]() {return s->state == dataAvailable || s->requestTerminate; });
					if (s->requestTerminate)
						return;
					int ret = sendToStage(*s, nextStage, s->data + 1);
					if (ret == 3)
						return;
					s->state = readyForInput;
					s->readyForInput.notify_one();
				}
			}
		}

		int sendIntoPipe(Pipe& p, int value)
		{
			{
				lock_guard<mutex> lk(p.m);
				p.dataCount++;
			}
			Stage dummy{};
			int result = sendToStage(dummy, *(p.head), value, true /*is Sender Main*/);
			if (result == 2)
			{
				lock_guard<mutex> lk(p.m);
				p.dataCount--;
				return 2;
			}
			return 1;
		}

		int getSingleOutputFromPipe(Pipe& p, int& result)
		{
			Stage& lastStage = *(p.tail);
			bool empty = false;
			{
				lock_guard<mutex> lk(p.m);
				if (p.dataCount <= 0)
					empty = true;
				else
					p.dataCount--;
			}
			if (empty) return 2;

			{
				unique_lock<mutex> ulk (lastStage.m);
				lastStage.dataAvailable.wait(ulk, [&]() {return lastStage.state == dataAvailable;
					});
				result = lastStage.data;
				lastStage.state = readyForInput;
				lastStage.readyForInput.notify_one();
			}
			return 1;
		}


		void createPipe(Pipe& p, int stageCount)
		{
			int index;
			Stage* pre{nullptr};

			p.stageCount = stageCount;
			p.dataCount = 0;

			for (index = 0; index <= stageCount; index++)
			{
				Stage* newStage = new Stage();
				newStage->state = readyForInput;
				if (index == 0)
				{
					p.head = newStage;
				}
				else
				{
					pre->next = newStage;
				}
				pre = newStage;
			}
			p.tail = pre;

			for (Stage* s = p.head; s->next != nullptr; s = s->next)
			{
				thread thr{ stageDemon, s };
				s->t = move(thr);
			}
		}

		void deletePipe(Pipe& p)
		{
			/*
			*  When I want to delete a pipe, the state of the stages is likely to be this:
			*  (pretend I have 5 stages, s1 to s4 are running stageDemon, s5 is controled by main thread
			* 
			*  s1   s2   s3   s4   s5
			*  \_____/   \_____/   |
			*     |         |      +--- in data available state
			*     |         +---------- in the middle of sending to next stage, waiting for next stage to become ready for input
			*     |                     in sendToStage function, s4 is waiting on s5's readyForInput. s3 is waiting on s4's mutex
			*     +-------------------- in ready for input state, in stageDemon function, in the while loop, waiting on own stage's dataAvailable
			* 
			* This is just likely, i.e. I want to exit the program, all current data in the pipe line are pushed toward the end
			* as much as possible. It's also possible that when I want to exit, s2 is in the middle of sending data to s3
			* while s4 is waiting on own dataAvailable.
			* In a word, if a stage thread can be
			* (1) waiting on own dataAvailable
			* or (2) waiting on next stage's readyForInput,
			* or (3) waiting on next stage's mutex 
			*        For example: s4 is in the middle of sending data to s5, s4 is waiting on s5's readyForInput, 
			*                     s4 will release s5's mutex but s4 is holding its own mutex while waiting, 
			*                     so if I add a new data to the pipe now, s3 is going to wait for s4's mutex
			*                     while s3 is holding on its own mutex, so if I add another data, s2 will be waiting for s3's mutex
			* or (4) waiting for its turn to run. It's just simply scheduled *out* and waiting for its turn to execute or resume.
			* 
			* How do I forcefully terminate all the stage threads?
			* I'm using a very brute force way. Set all the "requestTerminate" flag to true.
			* Notify on all the "dataAvailable" and "readyForInput" condition variable
			* Do all these without even bother locking the mutex.
			* If a stage thread is waiting on some condition variable, it's going to wake up and checking "requestTerminate" flag.
			* If a stage thread is just waiting for scheduler to give it its turn, it's going to get close to waiting on some condition variable
			* it's going to check the predicate once before actually waiting because cv.wait(lock, predicate) is equivalent to
			* while (!predicate)
			* {
			*     wait(lock);
			* }
			* What if a thread already did that very first check on predicate and then got suspended?
			* What if this is the sequence:
			* thread x checked predicate, it's false
			* thread x is suspended
			* deletePipe sets all requestTerminate and notify on all condition variables
			* thread x resumes running, executing wait(lock)
			* 
			* Will thread x not wait because there was a notification before?
			* Answer: No. See en.cppreference.com/w/cpp/thread/condition_variable/notify_one
			* If the above sequence happens, my last hope is spurious wake up.
			*/
			Stage* s = p.head;
			while (s->next != nullptr)
			{
				s->requestTerminate = true;
				s = s->next;
			}
			s = p.head;
			while (s != nullptr)
			{
				s->dataAvailable.notify_one();
				s->readyForInput.notify_one();
				s = s->next;
			}
			s = p.head;
			int index = 0;
			while (s->next != nullptr)
			{
				s->t.join();
				cout << "thread " << index << " joined\n";  // this output can be scrambled
				Stage* pre = s;
				s = s->next;
				index++;
				delete pre;
			}
		}

		void Test_Pipe()
		{
			Pipe p;
			int value, result;

			createPipe(p, 10);
			cout << "Enter integer values, or \"99\" for next result, or \"88\" to exit\n";

			while (true)
			{
				cout << "Data > ";
				cin >> value;
				if (value == 88)
				{
					deletePipe(p);
					break;
				}
				if (value == 99)
				{
					int status = getSingleOutputFromPipe(p, result);
					if (status == 1)
						cout << "result: " << result << '\n';
					else
						cout << "No result ready\n";
				}
				else
				{
					int ret = sendIntoPipe(p, value);
					if (ret == 2)
					{
						cout << "Pipe is full, dropping the last input value " << value << '\n';
					}
				}
			}
		}
	}


	namespace Pipe2 {

		using namespace std;

		/*
		*      stage0  -->  stage1 --> stage2  --> stage3
		*   6    +1     7    +1     8   +1      9    9
		* 
		* I call this a pipe with 3 stages.
		* 
		* A pipe with 3 stages will have 4 Stage objects
		* and 3 new threads. The main thread controls the find stage.
		* A pipe with x stages will increase the input x times.
		* A pipe with x stages can hold x+1 numbers.
		* 
		*/

		class Stage;

		enum class StageState
		{
			readyForInput = 0,
			dataAvailable = 1
		};

		enum class RetCode
		{
			ok = 0,
			empty = 1,
			shouldDrop = 2,
			shouldTerminate = 3,
		};

		class Pipe
		{
			vector<Stage> stages;
			bool pipeStarted;
			int dataCount;

		public:
			Pipe(int stageCount);
			
			~Pipe();

			void startPipe();

			Stage* getNextStage(int id_);

			Stage* getPrevStage(int id_);

			RetCode sendIntoPipe(int value);

			RetCode getOneResult(int& result);
		};

		class Stage
		{
			mutex m;
			condition_variable readyForInput;
			condition_variable dataAvailable;
			StageState state;
			thread t;
			int data;
			int id;
			Pipe& p;
			bool beingOrderedToTerminate;

		public:

			// Since I'm declaring "p" as a reference, I can't make a default constructor.

			Stage(int id_, Pipe& p_) : id(id_), p(p_), state{ StageState::readyForInput }, beingOrderedToTerminate{ false }
			{
			}

			Stage(Stage const& s_): p(s_.p), id(s_.id), state{s_.state}, beingOrderedToTerminate(s_.beingOrderedToTerminate)
			{
			}

			~Stage()
			{
				if (t.joinable())
					t.join();
			}
			void start()
			{
				t = thread(&Stage::stageDemon, this);
			}

			void stageDemon()
			{
				Stage* next = p.getNextStage(id);
				{
					unique_lock<mutex> ulk(m);
					while (true)
					{
						dataAvailable.wait(ulk, [&]() {return state == StageState::dataAvailable || beingOrderedToTerminate; });
						if (beingOrderedToTerminate)
							return;
						RetCode ret = next->sendToThis(data + 1);
						if (ret == RetCode::shouldTerminate)
							return;
						state = StageState::readyForInput;
						readyForInput.notify_one();
					}
				}
			}

			RetCode sendToThis(int value)
			{
				Stage* prev = p.getPrevStage(id);
				unique_lock<mutex> ulk(m, defer_lock);
				bool result = ulk.try_lock();
				if (!result && id==0)
					return RetCode::shouldDrop;
				else if (!result)
					ulk.lock();
				readyForInput.wait(ulk,
					[&]()
					{
						return (state == StageState::readyForInput) || (id>0 && prev->beingOrderedToTerminate);
					});
				if (id>0 && prev->beingOrderedToTerminate)
					return RetCode::shouldTerminate;
				data = value;
				state = StageState::dataAvailable;
				dataAvailable.notify_one();
				return RetCode::ok;
			}

			void getStageData(int& data_)
			{
				unique_lock<mutex> ulk(m);
				dataAvailable.wait(ulk, [&]() {return state == StageState::dataAvailable; });
				data_ = data;
				state = StageState::readyForInput;
				readyForInput.notify_one();
			}

			void shutStage1()
			{
				beingOrderedToTerminate = true;
			}
			void shutStage2()
			{
				readyForInput.notify_one();
				dataAvailable.notify_one();
			}
		};


		//---------------------------------------------------------
		// postponed definition

		Pipe::Pipe(int stageCount) : pipeStarted{ false }, dataCount{ 0 }
		{
			for (int i = 0; i <= stageCount; i++)
			{
				// if adding a new element to the vector causes the vector to resize, it going to use the element's copy constructor.
				stages.emplace_back(i, *this);
			}
		}

		Pipe::~Pipe()
		{
			/*
			* ATTENTION!!!  There is an intrinsic flaw in the way I shut down stage threads
			* (1). stage thread approach some "wait" statement
			* (2). stage thread check wait condition (the lambda function inside "wait" function call)
			* (3). condition returns false (means need to wait)
			* (4). main thread resumes and set beingOrderedToTerminate
			* (5). main thread continues and notifies both condition variables on all stages
			* (6). stage thread resumes, not this stage thread has missed the notification and will go into sleep waiting
			*/
			int len = stages.size();
			for (int i = 0; i < len; i++)
				stages[i].shutStage1();
			for (int i = 0; i < len; i++)
				stages[i].shutStage2();
		}

		void Pipe::startPipe()
		{
			for (int i = 0; i <= stages.size() - 2; i++)
				stages[i].start();
		}

		Stage* Pipe::getNextStage(int id_)
		{
			return &stages[id_ + 1];
		}

		Stage* Pipe::getPrevStage(int id_)
		{
			return id_ != 0 ? &stages[id_ - 1] : nullptr;
		}

		RetCode Pipe::sendIntoPipe(int value)
		{
			if (!pipeStarted)
			{
				startPipe();
				pipeStarted = true;
			}

			RetCode ret = stages[0].sendToThis(value);
			if (ret != RetCode::shouldDrop)
				dataCount++;
			return ret;
		}

		RetCode Pipe::getOneResult(int& result)
		{
			if (dataCount == 0)
				return RetCode::empty;

			auto it = stages.end() - 1;
			it->getStageData(result);
			dataCount--;
			return RetCode::ok;
		}


		//---------------------------------------------------------

		void Test_Pipe2()
		{
			Pipe p{ 1 };
			string input;
			int value;

			while (true)
			{
				cout << "Data > ";
				//cin >> value;
				input.clear();
				getline(cin, input);
				if (input.size()==0)
					break;
				
				if (input[0] == '=')
				{
					int result;
					RetCode ret = p.getOneResult(result);
					if (ret == RetCode::empty)
						cout << "No result ready\n";
					else
						cout << "result: " << result << '\n';
				}
				else
				{
					value = stoi(input);
					RetCode ret = p.sendIntoPipe(value);
					if (ret == RetCode::shouldDrop)
					{
						cout << "Pipe is full, dropping the last input value " << value << '\n';
					}
				}
			}
		}
	}



	namespace ReadWriteLock {

		using namespace std;
		
		mutex ioMtx;

		struct ReadWriteLock
		{
			shared_mutex m;
		};

		class DataABC
		{
			ReadWriteLock rwl;

		public:
			int data;
			int updates;
			DataABC() :data{ 0 }, updates{ 0 }, rwl{}
			{}

			~DataABC()
			{
			}
			void readLock()
			{
				rwl.m.lock_shared();
			}
			void readUnlock()
			{
				rwl.m.unlock_shared();
			}
			void writeLock()
			{
				rwl.m.lock();
			}
			void writeUnlock()
			{
				rwl.m.unlock();
			}
		};

		struct ThreadABC
		{
			thread t;
			int num;
			int updates;
			int reads;
			int interval;
		};

		void threadRoutine(ThreadABC* tABC, vector<DataABC>&dataVec)
		{
			int repeats{ 0 };
			for (int i = 0, j=0; i < 10'000; i++)
			{
				if (i % tABC->interval == 0)
				{
					dataVec[j].writeLock();
					dataVec[j].data = tABC->num;
					dataVec[j].updates++;
					tABC->updates++;
					dataVec[j].writeUnlock();
				}
				else
				{
					dataVec[j].readLock();
					tABC->reads++;
					if (dataVec[j].data == tABC->num)
						repeats++;
					dataVec[j].readUnlock();
				}
				j++;
				j %= dataVec.size();
			}
			{
				lock_guard<mutex> lk(ioMtx);
				cout << "thread " << tABC->num << " interval: " << tABC->interval << " repeats: " << repeats <<'\n';
			}
		}

		void Test_ReadWriteLock()
		{
			random_device rd;
			mt19937 rgen(rd());

			int const TotalDataCount = 120;
			int const TotalThreadCount = 4;

			vector<DataABC> d(TotalDataCount);
			vector<ThreadABC> th(TotalThreadCount);

			int index = 0;
			for (ThreadABC& a : th)
			{
				a.interval = rgen() % 71;
				a.num = index;
				thread b{threadRoutine, &a, std::ref(d)};
				a.t = std::move(b);
				index++;
			}

			int totalThreadUpdates = 0;
			for (ThreadABC& a : th)
			{
				a.t.join();
				totalThreadUpdates += a.updates;
			}

			int totalDataUpdates = 0;
			for (DataABC& a : d)
			{
				totalDataUpdates += a.updates;
			}

			cout << "total thread updates: " << totalThreadUpdates << "; total data updates: " << totalDataUpdates << '\n';
		}
	}


	namespace ClientServer {
		using namespace std;

		class Client; // forward declare
		
		enum class ReqType
		{
			QUIT = 0,
			READ = 1,
			WRITE = 2
		};

		struct Request
		{
			// I initially tried to declare all the string members as references in order to save time and space
			// by not making copies. But each Request object is essentially the data shared between threads
			// once an object is in the request list, it must not be modified by any thread without first securing
			// the mutex, if these data members are references, that is as if leaving a back door for a thread
			// to modify them without locking the mutex. The "input" member has to be a reference because
			// client thread needs to read it. Once server thread does pop_front, the Request object is destroyed.
			// The server thread may do pop_front before client thread wakes up.

			ReqType rtype;
			bool sync;
			string prompt;
			string& input;
			string output;
			
			bool done{ false };
			condition_variable thisReqFinished;

			Request(ReqType rtype_, bool sync_, string& prompt_, string& inputOrOutput) : 
				rtype{ rtype_ }, sync{ sync_ }, prompt{ prompt_ }, input{ inputOrOutput }, output{ inputOrOutput }
			{}
		};

		class Server
		{
			thread t;
			mutex m;
			condition_variable reqAvailable;

			bool running{ false };
			list<Request> reqs;

		public:
			Server() {}

			void serverRoutine()
			{
				while (true)
				{
					unique_lock<mutex> ulk (m, defer_lock);
					ulk.lock();
					reqAvailable.wait(ulk, [&]() {return reqs.size() > 0; });
					Request &r = reqs.front(); 
					// must not pop_front here, because that destroys the first element, I have yet to process that request.
					// Why do I use list instead of deque ro vector?
					// after server unlocks, client can do an emplace_back, according to documentation,
					// emplace_back on deque invalidates all iterators but references are not invalidated.
					// emplace_back on vector can invalidate all iterators and references if it needs to resize
					
					ulk.unlock();
										
					switch (r.rtype)
					{
					case ReqType::READ:
						cout << r.prompt << '\n';
						cin >> r.input;
						break;
					case ReqType::WRITE:
						cout << r.output << '\n';
						break;
					case ReqType::QUIT:
					default:
						break;
					}

					if (r.sync)
					{
						unique_lock<mutex> ulk(m);
						r.done = true;
						r.thisReqFinished.notify_one();
					}
					if (r.rtype == ReqType::QUIT)
						break;

					ulk.lock();
					reqs.pop_front();
					ulk.unlock();
				}
			}

			void addRequest(ReqType rtype, bool sync, string& prompt, string& inputOrOutput)
			{
				unique_lock<mutex> ulk(m);
				if (!running)
				{
					running = true;
					t = thread(&Server::serverRoutine, this);
					t.detach();
				}
				reqs.emplace_back(rtype, sync, prompt, inputOrOutput);
				reqAvailable.notify_one();
				if (sync)
				{
					Request& req = reqs.back();
					req.thisReqFinished.wait(ulk, [&]() { return req.done; });
				}
			}

			void addRequest(ReqType rtype, bool sync, string&& prompt, string&& inputOrOutput)
			{
				addRequest(rtype, sync, prompt, inputOrOutput);
			}

			void addRequest(ReqType rtype, bool sync)
			{
				addRequest(rtype, sync, "", "");
			}

		};

		class ClientGroup
		{
			vector<Client> cVec;
			mutex m;
			int activeClients;
			condition_variable allDone;

		public:
						
			ClientGroup(int clientsCount, Server& server_);

			~ClientGroup() {}
			
			void waitForAllDone()
			{
				unique_lock<mutex> lk(m);
				allDone.wait(lk, [&]() {return activeClients == 0; });
			}

			void reduceClient()
			{
				unique_lock<mutex> ulk(m);
				activeClients--;
				if (activeClients == 0)
					allDone.notify_one();
			}
		};

		class Client
		{
			ClientGroup& cg;
			Server& svr;
			thread t;
		public:
			Client(Server& svr_, ClientGroup& cg_) :svr(svr_), cg(cg_) {}

			Client(Client const& client_) : svr(client_.svr), cg(client_.cg) {} 
			// this copy constructor must be defined if I want to define a destructor.
			// If I don't define this, compiler will auto generate one
			// the auto generated one will copy every data member, but std::thread must not be copied.
			
			~Client()
			{
				if (t.joinable())
					t.join();
			}

			void start()
			{
				t = thread(&Client::clientRoutine, this);
				// must not do this inside constructor, because once this thread is created, it starts running
				// it may starts running even before svr and cg get their value initialized even if they are initialized using initializer list
			}
			void clientRoutine()
			{
				thread::id tid = t.get_id();
				stringstream ss;
				ss << tid;
				string prompt = ss.str();
				prompt = "Client " + prompt + " > ";

				while (true)
				{
					string input;
					svr.addRequest(ReqType::READ, true, prompt, input);
					
					if (input=="0") break;
					
					for (int i = 0; i < 4; i++)
					{
						string output = prompt + " ___ " + input + " ___ " + to_string(i);
						svr.addRequest(ReqType::WRITE, false, prompt, output);
						// this "output" here must not be a reference, because after a request is added to the deque,
						// before server thread gets a chance to process that request, the real memory of "output" can be destroyed or overwritten.
					}
				}

				cg.reduceClient();	
			}
		};


		/*
		* ClientGroup's member functions refer to Client
		* Client's member functions refer to ClientGroup
		* Whichever way to order these two classes' defintion won't satisfy the compiler.
		* I must move some member functions' definition behind both classes' declaration.
		*/
		ClientGroup::ClientGroup(int clientsCount, Server& s) :
			activeClients{ clientsCount } 
			, cVec(clientsCount, Client{s, *this})
			// If I use initializer list to initialize cVec, "Client" struct needs to have copy constructor
			// for example clientsCount is 5
            // this way of initializing the vector will first create 1 object of Client
			// it'll then copy that object 5 times, 
			// copy constructor of struct Client must be defined instead of letting compiler generate it
			// because std::thread must not be copied.
		{
			//for (int i = 0; i < clientsCount; i++)  // If I don't use initializer list, I can do it this way
				//cVec.emplace_back(s, *this);
			for (Client& c : cVec)
				c.start();
		}

		//------------------------------------------------------

		void Test_ClientServer()
		{
			Server server;
			ClientGroup cg{ 3, server };

			cg.waitForAllDone();

			server.addRequest(ReqType::QUIT, false/*sync*/);
		}
	}


	namespace Crew {
		using namespace std;

		/*
		* The key thing in this demo is that the work item list must not be empty
		* at any point in time unless all work items are processed and no new ones
		* are added. In another word, a worker must process an item first and then
		* remove it from the list.
		* This is because I'm using the emptyness of the work item list as an
		* indication for worker thread to terminate.
		* 
		* The implication of this decision is that worker thread must be aware
		* that work items are stored in a list instead of vector or deque, becase
		* worker thread gets back an iterator type when asking for a work item.
		* This implication is not ideal, worker thread should just get back a
		* WorkItem object when asking for work.
		*/

		class Crew;

		struct TreeNode
		{
			TreeNode* left {nullptr};
			TreeNode* right{ nullptr };
			int id{ 0 };
			vector<int> data = vector<int>(100'000, 0);
		};

		struct WorkItem
		{
			TreeNode* p{ nullptr };
			WorkItem(TreeNode* p_) :p(p_) {}
			WorkItem(WorkItem const& wi_) :p(wi_.p) {}
		};

		class Worker
		{
			thread t;
			Crew& crew;

		public:

			int found{ 0 };
			int id;

			Worker(int id_, Crew& crew_) :id(id_), crew(crew_) {}
			
			Worker(Worker const& w_) :id(w_.id), crew(w_.crew) {}

			~Worker()
			{
				if (t.joinable())
					t.join();
			}

			void start()
			{
				t = thread(&Worker::workerRoutine, this);
			}

			void workerRoutine();
		};

		class Crew
		{
			int crewSize;
			vector<Worker> wVec;
			mutex m;
			mutex iom;
			condition_variable allWorkersFinished;
			condition_variable newWorkWaiting;
			list<WorkItem> works;
			list<WorkItem>::iterator current;
			int aggregateFound{ 0 };
		public:
			int searchTarget;

			Crew(int crewSize_) : crewSize(crewSize_) 
			{
				for (int i = 0; i < crewSize_; i++)
				{
					wVec.emplace_back(i, *this);
				}
			}
			
			Crew(Crew const& crew_) : crewSize(crew_.crewSize) {}

			~Crew() 
			{
				cout << "Aggregate found: " << aggregateFound << '\n';
			}

			void start(TreeNode* n, int searchTarget_)
			{
				WorkItem wi{n};
				searchTarget = searchTarget_;
				
				works.push_back(wi);
				current = works.begin();
				for (Worker& w : wVec)
					w.start();
			}

			void addWorkItem(WorkItem& wi)
			{
				unique_lock<mutex> ulk(m);
				
				if (current == works.end())
					current = works.insert(current, wi);
				else
					works.push_back(wi);

				newWorkWaiting.notify_one();
			}

			void waitForDone()
			{
				unique_lock<mutex> ulk(m);
				allWorkersFinished.wait(ulk, [&]() {return crewSize == 0; });
			}

			int getWorkItem(list<WorkItem>::iterator& it)
			{
				unique_lock<mutex> ulk(m);
				
				newWorkWaiting.wait_for(ulk, 10ms, [&]() {return current != works.end(); });

				if (works.size() == 0)
					return -1;
				it = current;
				current = next(current);
				return 0;
			}

			void workerExit(Worker& w)
			{
				{
					unique_lock<mutex> ulk(iom);
					cout << "thread: " << w.id << " found: " << w.found << '\n';
				}
				unique_lock<mutex> ulk(m);
				aggregateFound += w.found;
				crewSize--;
				if (crewSize == 0)
					allWorkersFinished.notify_one();
			}

			void deleteWorkItem(list<WorkItem>::iterator& it)
			{
				unique_lock<mutex> ulk(m);
				works.erase(it);
			}
		};

		// ----------------------------------------------------------
		// postponed definition of Worker::workerRoutine

		void Worker::workerRoutine()
		{
			while (true)
			{
				list<WorkItem>::iterator it;
				int ret = crew.getWorkItem(it);
				if (ret == -1) break;
				// add new work item before processing this work item
				// so that other threads can get new work sooner
				if (it->p->left != nullptr)
				{
					WorkItem wi{it->p->left};
					crew.addWorkItem(wi);
				}
				if (it->p->right != nullptr)
				{
					WorkItem wi{ it->p->right };
					crew.addWorkItem(wi);
				}

				found += count(it->p->data.begin(), it->p->data.end(), crew.searchTarget);
				
				crew.deleteWorkItem(it);
			}
			crew.workerExit(*this);
		}


		// ----------------------------------------------------------

		void Test_Crew()
		{
			/*
			*               0
			*     +---------+-------------+
			*     1                       2
			*  +--+-------+       +-------+---------+
			*  3          5       6                 7
			*  +---+   +--+       +---+
			*      4   8              9
			*/
			TreeNode nodes[10];
			nodes[0].left = &nodes[1];
			nodes[0].right = &nodes[2];
			nodes[1].left = &nodes[3];
			nodes[1].right = &nodes[5];
			nodes[3].right = &nodes[4];
			nodes[5].left = &nodes[8];
			nodes[2].left = &nodes[6];
			nodes[2].right = &nodes[7];
			nodes[6].right = &nodes[9];
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution dist(1, 50);

			int id = 0;
			int total=0;
			for (auto i = begin(nodes); i != end(nodes); i = next(i))
			{
				i->id = id;
				generate(i->data.begin(), i->data.end(), std::bind(dist, ref(gen)));
				id++;
				total += count(i->data.begin(), i->data.end(), 28);
			}
			cout << "There are " << total << " occurrences of 28\n";
			

			Crew crew{4};

			crew.start(&nodes[0], 28);

			crew.waitForDone();

		}
	}


	namespace Crew2 {
		using namespace std;

		/*
		* 1. A worker can add any number of new work items while processing a work item.
		* For example, a work item has 10 children, a worker processes it, and adds 10 more
		* work items to the list, how many other worker threads should this thread wake up?
		* There is no way to tell. For example if there are 3 other active worker threads,
		* should this thread wake up 7? What if those 3 active are going to take long time
		* processing their current work? What if waking up 10? What if those 3 active finishe
		* soon? There is no way for this thread to determine the best number.
		* 
		* 2. A worker thread must wake up from time to time by itself, it can't just wait for
		* others to notify it.
		* 
		* 3. When a worker thread wakes up, it sees no work item in the list, how can this
		* thread distinguish these two scenarios (1). there is truly no more work, and this
		* thread should terminate (2). there are other active workers, they will add new
		* work items shortly after.
		* 
		* 4. When a worker wakes up, it must have a way to know how many active workers are
		* there at the moment, in case this worker sees no work item in list, it can wait
		* for active workers to add more work items.
		* 
		* 5. I don't want Worker class deal with the mutex. Mutex resides in Crew class.
		* Every function that deals with the mutex should be a member of the Crew class.
		* For example, getWorkItem is a member of the Crew class. This means once the execution
		* leaves the Crew class, the mutex is unlocked. At that moment, active workers count
		* must match the reality. 
		* 
		* 6. I also don't want a worker thread to keep track of its own activeness. I also
		* don't want the Crew class to keep track of individual worker's activeness. I just
		* want an aggregated count of active workers.
		* 
		* 7. How do I define a worker thread to be active? Does it have to be actively
		* processing a work item? Or is it active as long as it's not in sleep?
		* 
		* 8. The solution is to define activeness as not being in sleep. Therefore, decrement
		* active worker count right before a worker thread waits on condition variable.
		* If it doesn't need to wait and gets a work item right away, I'll increment back
		* the active worker count. If it does need to wait, then that's good because I already
		* decremented the counter. 
		* 
		* 9. Another important aspect resulting from this decision is that every thread starts
		* as active, because they are not in sleep. 
		*/

		class Crew;

		struct TreeNode
		{
			TreeNode* left{ nullptr };
			TreeNode* right{ nullptr };
			int id{ 0 };
			vector<int> data = vector<int>(100'000, 0);
		};

		struct WorkItem
		{
			TreeNode* p{ nullptr };
			WorkItem() {}
			WorkItem(TreeNode* p_) :p(p_) {}
			WorkItem(WorkItem const& wi_) :p(wi_.p) {}
		};

		class Worker
		{
			thread t;
			Crew& crew;

		public:

			int found{ 0 };
			int id;

			Worker(int id_, Crew& crew_) :id(id_), crew(crew_) {}

			Worker(Worker const& w_) :id(w_.id), crew(w_.crew) {}

			~Worker()
			{
				if (t.joinable())
					t.join();
			}

			void start()
			{
				t = thread(&Worker::workerRoutine, this);
			}

			void workerRoutine();
		};

		class Crew
		{
			vector<Worker> wVec;
			mutex m;
			mutex iom;
			condition_variable allWorkersFinished;
			condition_variable newWorkAdded;
			list<WorkItem> works;

			int aggregateFound{ 0 };
			int activeWorker;
			int crewSize;
			int workerExited{ 0 };

		public:
			int searchTarget;

			Crew(int crewSize_): activeWorker{crewSize_}, crewSize(crewSize_)
			{
				for (int i = 0; i < crewSize_; i++)
				{
					wVec.emplace_back(i, *this);
				}
			}

			Crew(Crew const& crew_) {}

			~Crew()
			{
				cout << "Aggregate found: " << aggregateFound << '\n';
			}

			void start(TreeNode* n, int searchTarget_)
			{
				WorkItem wi{ n };
				searchTarget = searchTarget_;

				works.push_back(wi);

				for (Worker& w : wVec)
					w.start();
			}

			void addWorkItem(WorkItem& wi)
			{
				unique_lock<mutex> ulk(m);

				works.push_back(wi);

				newWorkAdded.notify_one();
			}

			void waitForDone()
			{
				unique_lock<mutex> ulk(m);
				allWorkersFinished.wait(ulk, [&]() {return workerExited == crewSize; });
			}

			int getWorkItem(WorkItem&wi)
			{
				unique_lock<mutex> ulk(m);
				activeWorker--;
				newWorkAdded.wait_for(ulk, 10ms, [&]() {return works.size()>0 || activeWorker==0; });

				if (works.size() > 0)
				{
					WorkItem& item = works.front();
					wi = item;
					activeWorker++;
					works.pop_front();
					return 1;
				}
				if (activeWorker == 0)
					return 0;

				return -1;
			}

			void workerExit(Worker& w)
			{
				{
					unique_lock<mutex> ulk(iom);
					cout << "thread: " << w.id << " found: " << w.found << '\n';
				}
				unique_lock<mutex> ulk(m);
				aggregateFound += w.found;
				workerExited++;
				if (workerExited == crewSize)
					allWorkersFinished.notify_one();
			}
		};

		// ----------------------------------------------------------
		// postponed definition of Worker::workerRoutine

		void Worker::workerRoutine()
		{
			while (true)
			{
				WorkItem wi;
				
				int ret = crew.getWorkItem(wi);
				
				if (ret == 0 || ret == -1) break;
				
				if (wi.p->left != nullptr)
				{
					WorkItem newItem{ wi.p->left };
					crew.addWorkItem(newItem);
				}
				if (wi.p->right != nullptr)
				{
					WorkItem newItem{ wi.p->right };
					crew.addWorkItem(newItem);
				}

				found += count(wi.p->data.begin(), wi.p->data.end(), crew.searchTarget);

			}
			crew.workerExit(*this);
		}


		// ----------------------------------------------------------

		void Test_Crew()
		{
			/*
			*               0
			*     +---------+-------------+
			*     1                       2
			*  +--+-------+       +-------+---------+
			*  3          5       6                 7
			*  +---+   +--+       +---+
			*      4   8              9
			*/
			TreeNode nodes[10];
			nodes[0].left = &nodes[1];
			nodes[0].right = &nodes[2];
			nodes[1].left = &nodes[3];
			nodes[1].right = &nodes[5];
			nodes[3].right = &nodes[4];
			nodes[5].left = &nodes[8];
			nodes[2].left = &nodes[6];
			nodes[2].right = &nodes[7];
			nodes[6].right = &nodes[9];
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution dist(1, 50);

			int id = 0;
			int total = 0;
			for (auto i = begin(nodes); i != end(nodes); i = next(i))
			{
				i->id = id;
				generate(i->data.begin(), i->data.end(), std::bind(dist, ref(gen)));
				id++;
				total += count(i->data.begin(), i->data.end(), 28);
			}
			cout << "There are " << total << " occurrences of 28\n";


			Crew crew{ 4 };

			crew.start(&nodes[0], 28);

			crew.waitForDone();

		}
	}
}