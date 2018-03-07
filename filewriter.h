#pragma once

namespace core {

class FileWriter {
	FILE* fp = nullptr;
public:
	std::string path;
	ulong bytesWritten = 0;

	FileWriter(const std::string& path, bool append=false) : path(path) {
		auto mode = append ? "a" : "w";
		throwOnFileError(fopen_s(&fp, path.c_str(), mode));
	}
	~FileWriter() {
		if(fp) {
			fclose(fp);
			fp = nullptr;
		}
	}
	auto& write(ubyte value) {
		fwrite(&value, 1, 1, fp);
		bytesWritten++;
		return *this;
	}
	auto& write(ushort value) {
		fwrite(&value, 2, 1, fp);
		bytesWritten += 2;
		return *this;
	}
	auto& write(uint value) {
		fwrite(&value, 4, 1, fp);
		bytesWritten += 4;
		return *this;
	}
	auto& write(ulong value) {
		fwrite(&value, 8, 1, fp);
		bytesWritten += 8;
		return *this;
	}
	auto& write(float value) {
		fwrite(&value, 4, 1, fp);
		bytesWritten += 4;
		return *this;
	}
	auto& write(double value) {
		fwrite(&value, 8, 1, fp);
		bytesWritten += 8;
		return *this;
	}
	auto& write(const char* value) {
		if(value) {
			auto len = strlen(value);
			if(len>0) 
				fwrite(value, 1, len, fp);
			bytesWritten += len;
		}
		return *this;
	}
	auto& write(const std::string& value) {
		auto len = value.size();
		if(len>0)
			fwrite(value.data(), 1, len, fp);
		bytesWritten += len;
		return *this;
	}
	auto& writeFmt(const char* fmt, ...) {
		/// Use this for small strings
		char text[512];
		va_list	ap;
		va_start(ap, fmt);
		/// Count number of characters required
		int count = _vscprintf(fmt, ap) + 1;
		char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
		vsprintf_s(buf, count, fmt, ap);
		va_end(ap);

		/// Only write if we have more than "\0" in our string
		if(count > 1) {
			fwrite(buf, 1, count - 1, fp);
			fflush(fp);
			bytesWritten += (count-1);
		}

		if(buf != text) free(buf);
		return *this;
	}
	void flush() {
		fflush(fp);
	}
};

} /// core