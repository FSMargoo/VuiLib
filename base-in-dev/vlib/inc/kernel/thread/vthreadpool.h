/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vthreadpool.h
 * \brief A simple thread pool
 */

#pragma once

#include <condition_variable>
#include <functional>
#include <future>

#include <kernel/container/varray.h>
#include <kernel/container/vqueue.h>

/**
 * \brief VThread provide a thread pool which is used for VThread class
 */
class VThreadPool {
public:
	explicit VThreadPool(VMemoryPool &Allocator, const size_t &ThreadCount = std::thread::hardware_concurrency());
	template <class FunctionType, typename... Args>
	auto Summit(FunctionType &&Function, Args &&...Argument) {
		using ReturnType									   = std::invoke_result_t<FunctionType, Args...>;
		std::shared_ptr<std::packaged_task<ReturnType()>> Task = std::make_shared<std::packaged_task<ReturnType()>>(
			std::bind(std::forward<FunctionType>(Function), std::forward<Args>(Argument)...));
		std::future<ReturnType> Result = Task->get_future();
		{
			std::unique_lock<std::mutex> ThreadLock(QueueMutex);
			if (StopFlag) {
				_vdebug_handle().crash("Summit task on a stopped thread pool!");
			}

			Tasks.Push([Task = std::move(Task)]() { (*Task)(); });
		}

		Condition.notify_one();
		return Result;
	}
	~VThreadPool();

private:
	VArray<std::thread>			  Threads;
	VQueue<std::function<void()>> Tasks;
	std::mutex					  QueueMutex;
	std::condition_variable		  Condition;
	bool						  StopFlag;
};