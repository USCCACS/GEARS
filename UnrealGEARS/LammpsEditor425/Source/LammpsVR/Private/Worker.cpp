// Fill out your copyright notice in the Description page of Project Settings.

#include "Worker.h"
// #include "LammpsVR.h"

std::mutex Worker::m_internalLock;

Worker::Worker()
{
	m_thread = nullptr;
	m_signalLock = nullptr;
}

Worker::~Worker()
{
}

void
Worker::SetSignalLock(std::mutex* lock_) {
	m_signalLock = lock_;
}

#pragma region synch
void
Worker::LockThread() {
	m_internalLock.lock();
}

void
Worker::UnlockThread() {
	m_internalLock.unlock();
}

void
Worker::EnsureCompletion() {
	if (m_thread) m_thread->WaitForCompletion();
}

void
Worker::SignalCompletion() {
	if (m_signalLock) m_signalLock->unlock();
}
#pragma endregion synch


#pragma region unused

bool
Worker::Init() {
	return true;
}

void
Worker::Stop() {
	return;
}

#pragma endregion unused
