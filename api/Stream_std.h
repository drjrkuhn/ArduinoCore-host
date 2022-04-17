#pragma once

#include <iostream>
#include <sstream>
#include <mutex>
#include "Stream.h"

namespace arduino {

	template <class IOSTREAM>
	class Stream_stdstream : public Stream {
	public:
		Stream_stdstream(IOSTREAM& ios) : _ios(ios) {
			init(_ios);
		}
		IOSTREAM& ios() { 
			std::lock_guard<std::mutex> _(_guard); 
			return _ios; 
		}

		virtual size_t write(const uint8_t byte) override {
			std::lock_guard<std::mutex> _(_guard); 
			char cc = static_cast<char>(byte);
			return _canput && _ios.rdbuf()->sputc(cc) == cc ? 1 : 0;
		}
		virtual size_t write(const uint8_t* str, size_t n) override {
			std::lock_guard<std::mutex> _(_guard);
			return _canput ? _ios.rdbuf()->sputn(reinterpret_cast<const char*>(str), n) : 0;
		}
		virtual int availableForWrite() override {
			std::lock_guard<std::mutex> _(_guard); 
			return _canput ? std::numeric_limits<int>::max() : 0;
		}

		virtual int available() override {
			std::lock_guard<std::mutex> _(_guard); 
			return static_cast<int>(_canget ? _ios.rdbuf()->in_avail() : 0);
		}

		virtual int read() override {
			_guard.lock();
			int ret = static_cast<int>(_canget ? _ios.rdbuf()->sbumpc() : -1);
			_guard.unlock();
			update_buf();
			return ret;
		}
		virtual int peek() override {
			_guard.lock();
			int ret = static_cast<int>(_canget ? _ios.rdbuf()->sgetc() : -1);
			_guard.unlock();
			update_buf();
			return ret;
		}
		virtual size_t readBytes(char* buffer, size_t length) override {
			_guard.lock();
			size_t ret = _canget ? _ios.rdbuf()->sgetn(buffer, length) : 0;
			_guard.unlock();
			update_buf();
			return ret;
		}

	protected:
		virtual void update_buf() {}

		void init(IOSTREAM& ios) {  
			std::lock_guard<std::mutex> _(_guard); 
			_canget = ios.tellg() >= 0; 
			_canput = ios.tellp() >= 0; 
		}

		// protected default constructor for derived
		struct no_init_tag {};
		Stream_stdstream(IOSTREAM& ios, no_init_tag) : _ios(ios) {}

		IOSTREAM& _ios;
		bool _canget, _canput;
		mutable std::mutex _guard;
	};

	class Stream_stdstring : public Stream_stdstream<std::stringstream> {
	public:
		Stream_stdstring(
			const std::string str, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out | std::ios_base::app)
			: Stream_stdstream(_ss, (no_init_tag())), _ss(str, which) {
			init(_ss);
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		Stream_stdstring(std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
			: Stream_stdstream(_ss, (no_init_tag())), _ss(which) {
			init(_ss);
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		std::string str() const { 
			std::lock_guard<std::mutex> _(_guard); 
			return _ss.str(); 
		}
		void str(const std::string s) { 
			std::lock_guard<std::mutex> _(_guard); 
			_ss.str(s); 
		}
		void clear() { 
			std::lock_guard<std::mutex> _(_guard); 
			_ss.str(""); 
		}

		/** Diagnostic version of str. Shows g and p pointers. */
		std::string buffer_str() const {
			std::lock_guard<std::mutex> _(_guard); 
			std::string buf = _ss.str();
			// convert to signed positions
			long len = static_cast<long>(buf.length());
			long g = static_cast<long>(_ios.tellg());
			long p = static_cast<long>(_ios.tellp());
			if (p < 0)
				p = len;
			bool samegp = g == p;
			if (g < 0)
				g = 0;
			long headlen = g;
			long taillen = len - p;
			std::string ptrs;
			if (headlen - 1 > 0)
				ptrs.append((headlen - 1), '.');
			ptrs.append(1, samegp ? '@' : '^');
			ptrs.append(buf.substr(g, p - g));
			if (!samegp)
				ptrs.append(1, 'v');
			return ptrs;
		}

	protected:
		#if 1
		virtual void update_buf() override {
			std::lock_guard<std::mutex> _(_guard);
			std::streampos g = _ios.tellg(), p = _ios.tellp();
			// if (p < 0) 
			// 	p = _ss.str().length();
			if (g > 0 && g == p) {
				// _ss.rdbuf()->pubseekoff(0, std::ios_base::beg, std::ios_base::in | std::ios_base::out);
				// _ios_rdbuf()->pubsync();
				// _ss.seekp(0);
				// _ss.seekg(0);
				// _ss.pubseekpos(0, std::ios_base::in | std::ios_base::out);
				// _ss.str(""); // clear the string to prepare for more input
				// _ss.rdbuf()->pubseekoff(0, std::ios_base::beg, std::ios_base::in | std::ios_base::out);
			}
		}
		#endif
		std::stringstream _ss;
	};

} // namespace arduino