// Fill out your copyright notice in the Description page of Project Settings.

#include "LammpsVR.h"
#include "ThreadWorker.h"

std::mutex ThreadWorker::m_internalLock;

ThreadWorker::ThreadWorker()
{
}

ThreadWorker::~ThreadWorker()
{
}

void
ThreadWorker::SetSignalLock(std::mutex* lock_) {
	m_signalLock = lock_;

}

#pragma region synch
void
ThreadWorker::LockThread() {
	m_internalLock.lock();
}

void
ThreadWorker::UnlockThread() {
	m_internalLock.unlock();
}

void
ThreadWorker::EnsureCompletion() {
	if (m_thread) m_thread->WaitForCompletion();
}

void
ThreadWorker::SignalCompletion() {
	if (m_signalLock) m_signalLock->unlock();
}
#pragma endregion synch


#pragma region unused

bool
ThreadWorker::Init() {
	return true;
}

void
ThreadWorker::Stop() {
	return;
}

#pragma endregion unused
