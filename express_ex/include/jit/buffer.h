#ifndef BUFFER_H
#define BUFFER_H

#include "ParameterIO.h"
#include "common.h"
#include "variable.h"



enum class BufferTypeEn {
	input,
	internal,
	output
};


class Buffer {
public:

	Buffer(Variable * var) {
		length_           = var->getBufferLen();
		left_offset_	  = var->getLeftBufferLen();
		right_offset_	  = var->getRightBufferLen();
		type_			  = TypeEn::unknown_jty;
		type_			  = var->getType();
		sizeof_data_type_ = sizeOfTy(type_);
		setBufferAlloca();
	}

	~Buffer(){
		delete[] ptr_;
	}

	virtual int64_t init(){
		return 0;
	}

	virtual int64_t update(){
		memcpy(top_ptr_, ptr_, (left_offset_ + right_offset_));
		return 0;
	}

	void setBufferAlloca() {

		ptr_ = new char[sizeof_data_type_ * (length_ + left_offset_ + right_offset_)];
		bottom_ptr_ = ptr_ + (left_offset_ + right_offset_) * sizeof_data_type_;
		top_ptr_	= ptr_ + length_;
	}

	virtual BufferTypeEn getBufferType() {
		return BufferTypeEn::internal;
	}
	
	virtual std::string  getName() { 
		return "internal"; 
	}

	char *  getPtr() {
		return ptr_ ;
	}

	virtual int64_t close() {
		return 0;
	}

	template <typename T>
	T &stream(T &OS, std::string offset="") {
		OS << offset << "BufferInfo{\n";
		OS << offset << "  ptr: " << ptr_ << "\n";
		OS << offset << "  length: 0x";
		OS.write_hex(length_);
		OS << "\n";
		OS << offset << "  left_offset: " << left_offset_ << "\n";
		OS << offset << "  right_offset: " << right_offset_ << "\n";
		OS << offset << "  type: " << toString(type_) << "\n";
		//OS << offset << "  name: " << getName() << "\n";
		OS << offset << "  name: " << getName() << "\n";
		OS << offset << "}\n";
		return OS;
	}


protected:
	TypeEn  type_=TypeEn::unknown_jty;
	int64_t	sizeof_data_type_=-1;
	int64_t	length_=-1;
	int64_t	left_offset_=-1;
	int64_t	right_offset_=-1;
	
	char* ptr_ =NULL;
	char* bottom_ptr_=NULL;
	char* top_ptr_=NULL;
	std::string		link_name_ = std::string();
};



class InputBuffer: public Buffer {

public:
	InputBuffer( Variable * var):Buffer(var) {
		parameter_ = var->getPatameter();
	}
	~InputBuffer(){ }

	virtual int64_t init(){
		parameter_->open();
		parameter_->read(ptr_, left_offset_ + length_ + right_offset_);
		return 0;
	}

	virtual int64_t update() {
		memcpy(top_ptr_, ptr_, (left_offset_ + right_offset_));
		parameter_->read(bottom_ptr_, length_);
		return 0;
	}

	virtual int64_t close() {
		parameter_->close();
		return 0;
	}

	virtual BufferTypeEn getBufferType() {
		return BufferTypeEn::input;
	}

	virtual std::string  getName() { return parameter_->getName(); }

protected:
	SyncParameter * parameter_ = NULL;

};



class OutputBuffer : public Buffer {

public:
	OutputBuffer(Variable * var) :Buffer(var) {
		parameter_ = var->getPatameter();
	}

	~OutputBuffer() { 
		delete parameter_;
	}

	void addParameter(SyncParameter * parameter){
		parameter_ = parameter;
	}

	virtual int64_t init() {
		parameter_->open(true);
		return 0;
	}

	virtual int64_t update() {
		parameter_->write(ptr_, length_ );
		return 0;
	}

	virtual int64_t close() {
		parameter_->close();
		return 0;
	}

	virtual BufferTypeEn getBufferType() {
		return BufferTypeEn::output;
	}

	virtual std::string  getName() { return parameter_->getName(); }
protected:
	SyncParameter * parameter_=NULL;
	std::string link_name_="";
};


#endif