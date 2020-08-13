
#include <sstream>

#include "buffer.h"
#include "body.h"
#include "table.h"
#include "llvmHdrs.h"
#include "IR_generator.h"
#include "printer.h"

using namespace llvm;


SubBlock::SubBlock(Variable * var)
{
    leftLength=var->getLeftBufferLen();
    rightLength=var->getRightBufferLen();
    length=var->getLength();
    setUint(var);
}

void SubBlock::setUint(Variable * var)
{
    uintList.push(var);
}

string SubBlock::print()
{
    const size_t max_line_length=90;
    string out=std::string(4, ' ')+"L,R: " + std::to_string(leftLength) + "," + std::to_string(rightLength) + "\n";
    for (auto i : uintList) {
        std::string txt       = i->printUint() + ";" + (i->isBuffered() ? " store" : "");
        std::string txtShifts = std::to_string(i->getLeftBufferLen()) + " : " + std::to_string(i->getRightBufferLen());
        std::string txtSkip   = std::string(max_line_length - ((txt.length() > max_line_length) ? 0 : txt.length()), ' ');

        out+=std::string(6, ' ') + txt + txtSkip + txtShifts + "\n";
    }
    return out;
}

bool SubBlock::generateIR(IRGenerator & builder, CycleStageEn type, std::string basicBlockPrefix, std::string basicBlockPostfix)
{
    LLVMContext & context = builder.getContext();

    std::string  levelTxt=basicBlockPostfix;

    BasicBlock* bbLoad  = BasicBlock::Create(context, "load_"   + basicBlockPrefix + levelTxt, builder.getCurrentFunction());
    BasicBlock* bbCalc  = BasicBlock::Create(context, "calc_"   + basicBlockPrefix + levelTxt, builder.getCurrentFunction());
    BasicBlock* bbStore = BasicBlock::Create(context, "store_"   + basicBlockPrefix + levelTxt, builder.getCurrentFunction());

    BasicBlock* bbLastStore=builder.getStoreBlock();

    if (NULL != bbLastStore)
    {
        builder.CreateStartBRs();
        builder.CreateCondBr(builder.getCurrentCMPRes(), builder.getLoadBlock(), bbLoad);
    }

    builder.ClearInitializedVariablesList();

    builder.SetLoadInsertPoint(bbLoad);
    builder.SetCalcInsertPoint(bbCalc);
    builder.SetStoreInsertPoint(bbStore);
        
    builder.SetCalcInsertPoint();


    Value* alloc =builder.CreatePositionalOffset(basicBlockPrefix + levelTxt, -leftLength);
    builder.SetStoreInsertPoint();
    Value* nextOffset =builder.CreateAdd(builder.getCurrentOffsetValue(), builder.getInt64(1));
    builder.CreateStore(nextOffset, alloc);
    builder.SetCurrentCMPRes(
        builder.CreateICmpULT(
            nextOffset,
            builder.getInt64(bufferLength + rightLength)));
    builder.SetCalcInsertPoint();

    for (auto i : uintList)
        i->setupIR(builder);

    return true;
}

//Table::Block section 
//
//
//
Block::Block(Variable* var) {
    level=var->getLevel();
    length=var->getLength();
    setUint(var);
}

void Block::setUint(Variable * var){
    uintList.push(var);
    setUintToSubtable(var);
}

void Block::setBufferLength(uint64_t bufferLength_) {
    bufferLength=bufferLength_;
    for (auto i : subBlockList)
        i->setBufferLength(bufferLength_);
    for (auto i : uintList) {
        i->setBufferLength(bufferLength_);
    }
}

void Block::setUintToSubtable(Variable * var)
{
    auto  leftBufferLen= var->getLeftBufferLen();
    auto  rightBufferLen= var->getRightBufferLen();
     
    for (auto i : subBlockList)
        if ((i->getLeftLength() == leftBufferLen) && (i->getRightLength() == rightBufferLen)) {
            i->setUint(var);
            return;
        }

    subBlockList.push(new SubBlock(var));
}

string  Block::print() {
    const size_t max_line_length=90;
    string out=std::string(2, ' ')+"level: " + std::to_string(level) + "\n";
    for (auto i : subBlockList) {
        out+=i->print();
    }
    /*
    for (auto i : uintList) {
        std::string txt       = i->printUint() + ";" + (i->isBuffered() ? " store" : "");
        std::string txtShifts = std::to_string(i->getLeftBufferLen()) + " : " + std::to_string(i->getRightBufferLen());
        std::string txtSkip   = std::string(max_line_length - ((txt.length() > max_line_length) ? 0 : txt.length()), ' ');

        out+="\t\t" + txt+ txtSkip+ txtShifts+ "\n";
    }
   */
    return out;
}


bool Block::generateIR(IRGenerator &builder, CycleStageEn type, std::string basicBlockPrefix)
{
    LLVMContext & context = builder.getContext();

    std::string  levelTxt=std::to_string(level);

    if ( (type == CycleStageEn::midle) || (type == CycleStageEn::end) ){
        
        if ((type == CycleStageEn::end) & (0 == ( length % bufferLength )))
            return false;


        BasicBlock* bbIntermediate  = BasicBlock::Create(context, "intermediate_" + basicBlockPrefix + levelTxt, builder.getCurrentFunction());
        BasicBlock* bbLoad  = BasicBlock::Create(context, "load_" + basicBlockPrefix+ levelTxt, builder.getCurrentFunction());
        BasicBlock* bbCalc  = BasicBlock::Create(context, "calc_" + basicBlockPrefix + levelTxt, builder.getCurrentFunction());
        BasicBlock* bbStore = BasicBlock::Create(context, "store_" + basicBlockPrefix +  levelTxt, builder.getCurrentFunction());


        BasicBlock* bbLastStore=builder.getStoreBlock();

        if (NULL != bbLastStore)
        {
            builder.CreateMidleBRs();
            builder.CreateCondBr(builder.getCurrentCMPRes(), builder.getLoadBlock(), bbIntermediate);
  
        }
        else {
            builder.SetLoopEnterInsertPoint();
            builder.CreateBr(bbIntermediate);
        }

        builder.ClearInitializedVariablesList();

        builder.SetIntermediateInsertPoint(bbIntermediate);
        builder.SetOffsetToZero();

        builder.SetLoadInsertPoint(bbLoad);
        builder.SetCalcInsertPoint(bbCalc);
        builder.SetStoreInsertPoint(bbStore);


        
        builder.SetLoadInsertPoint();

        Value* offsetAlloc = builder.getCurrentOffsetValueAlloca();
        Value* offset= builder.CreateLoadOffset();

        builder.SetStoreInsertPoint();

        Value* nextOffset =builder.CreateAdd(offset, builder.getInt64(1));
        builder.CreateStore(nextOffset, offsetAlloc);

        int len=(type == CycleStageEn::midle) ? bufferLength : length % bufferLength ;
        builder.SetCurrentCMPRes(
            builder.CreateICmpULT(
                nextOffset,
                builder.getInt64(len)));
        builder.SetCalcInsertPoint();


        for (auto i : uintList)
            i->setupIR(builder);

    }else if(type == CycleStageEn::start){
        std::string  levelTxt=std::to_string(level);
        size_t sub_level=0;
        for (auto i : subBlockList) {
            i->generateIR(builder, type, basicBlockPrefix, levelTxt+"_"+std::to_string(sub_level));
            sub_level++;
        }

    }
    return true;
}


//TableColumn:: column section 
//
//
TableColumn::TableColumn (Variable* var) {
    length =var->getLength();
    setUint(var);
}

void    TableColumn::setUint(Variable * var){
    auto  varlevel= var->getLevel();
    for (auto i : blockList) 
        if (i->getLevel() == varlevel){
            i->setUint(var);
            return;
        }
    blockList.push(new Block(var));
}

string  TableColumn::print(){

    string out="length: " + std::to_string(length) + "\n";
    for (auto i : blockList) {
        out+= i->print();
    }
    return out;
}

bool    TableColumn::generateIR(IRGenerator &builder, CycleStageEn type, std::string basicBlockPrefix) {
    std::stringstream hex;
    bool res=false;
    for (auto i : blockList) {
        std::stringstream ss;
        ss<< std::hex <<length;

        res|=i->generateIR(builder, type, basicBlockPrefix + "block_" +"0x" +ss.str() + "_level_");
    }
    return res;
}



template <typename T>
T convolveTemplate(T * i, T * j){
    return (T)100;
}
//convolveTemplate<int64_t>
int64_t convolveI64(int64_t * i, int64_t * j) {
    return 100;
}
int32_t convolveI32(int32_t * i, int32_t * j) {
    return 100;
}
int16_t convolveI16(int16_t * i, int16_t * j) {
    return 100;
}
double convolveDouble(double * i, double * j) {
    return 100.0;
}
float convolveFloat(float * i, float * j) {
    return 100.0;
}

Value * genConvolve(IRGenerator & builder, llvm::Type* type, uintptr_t addr) {

    Value *convolveFunction = builder.CreateIntToPtr(ConstantInt::get(builder.getInt64Ty(),
        uintptr_t(addr)),
        PointerType::getUnqual(
            FunctionType::get(builder.getInt64Ty(),
                { type->getPointerTo(), type->getPointerTo() },
                false
            )
        ));
    //convolveFunction->setName("convolve");
    return  convolveFunction;
}


std::vector <Buffer* >   *g_buffers=NULL;


ParametersDB*  g_IO_IFS =NULL;

bool isInvalidBuffers() {
    return 
        NULL == g_buffers ;
}

int32_t initBuffers() {
    if (isInvalidBuffers())
        return 1;
    for (auto i : *g_buffers)
        i->init();
    return 0;
}

int32_t updateBuffer() {
    if (isInvalidBuffers()) 
        return 1;
    for (auto i : *g_buffers)
        i->update();
    return 0;
}


///Table:: Table section 
///

// add variable to table 
void    Table::setUint(Variable * var){
    auto  varLength= var->getLength();
    if (isConst(var)) {
        constList.push(var);
        return;
    }
    if (isSmallArr(var)) {
        smallArrayList.push(var);
        return;
    }
    for (auto i : columnList_)
        if (i->getLength() == varLength) {
            i->setUint(var);
            return;
        }
    columnList_.push(new TableColumn(var));
}

llvm::Function * Table::getFloatBIFunc(OpCodeEn op) { 
    //return floatBIFuncMap[op];
    return getBIFunc (floatBIFuncMap, op, fTy);
}

llvm::Function * Table::getDoubleBIFunc(OpCodeEn op) {
    //return doubleBIFuncMap[op]; 
    return getBIFunc (doubleBIFuncMap, op, dTy);
}

llvm::Function*  Table::getBIFunc(BuiltInFuncMap &UBIFMap, OpCodeEn op, Type * Ty) {
    auto pos=floatBIFuncMap.find(op);
    if (pos != floatBIFuncMap.end()) {
        return pos->second;
    }
    else {
        Intrinsic::ID code =BIF2LLVMmap_[op];
        Function* f=Intrinsic::getDeclaration(M, code, Ty);
        (UBIFMap)[op] = f;
        return f;
    }
}

void    Table::declareFunctions() {
    LLVMContext & context = M->getContext();
    fTy = Type::getFloatTy(context);
    dTy = Type::getDoubleTy(context);
    declareBuiltInFunctions(floatBIFuncMap, fTy);
}

void    Table::declareBuiltInFunctions(BuiltInFuncMap &UBIFMap, Type * Ty) {
    BIF2LLVMmap_ ={
{OpCodeEn::fpow, Intrinsic::pow},
{OpCodeEn::cos, Intrinsic::cos},
{OpCodeEn::sin, Intrinsic::sin},
{OpCodeEn::exp, Intrinsic::exp},
{OpCodeEn::log, Intrinsic::log},
{OpCodeEn::log2, Intrinsic::log2},
{OpCodeEn::log10,Intrinsic::log10} };
}


string  Table::print() {
    string out="";

    out+="constant defenition:\n";
    for (auto i : constList) {
        out+="\t" + i->printUint() + ";\n";
    }
    out+="small array defenition:\n";
    for (auto i : smallArrayList) {
        out+="\t" + i->printUint() + ";\n";
    }
    out+="table:\n";
    for (auto i : columnList_) {
        out+= i->print();
    }
    return out;
}

void Table::calculateBufferLength(std::string basicBlockPrefix){

    int maxLength=0;
    int minLength=columnList_[0]->getLength();

    typedef struct {
        uint64_t maxLength;
        uint64_t minLength;
        uint64_t iterations;
        stack<TableColumn *> columnList;
    } group;

    std::vector<group> groupList;

    for (auto  i : columnList_){
        uint64_t length=i->getLength();
        bool exist=false;
       
        for (auto &j : groupList) {
            auto remainder =(j.maxLength > length) ? j.maxLength % length : length % j.maxLength;
            if (remainder == 0) {
                exist=true;
                j.maxLength=length > j.maxLength ? length : j.maxLength;
                j.minLength=length < j.minLength ? length : j.minLength;
                j.columnList.push_back(i);
                break;
            }
        }

        if (!exist) {
            group g={ length ,length };
            g.columnList.push_back(i);
            groupList.push_back(g);
        }
    }

    for (auto &j : groupList) {

        int subMaxBufferLength = minBufferLength_ * j.maxLength / j.minLength;
        
        if (subMaxBufferLength > maxBufferLength_) {
            llvm::outs() << "maxBufferLength = subMaxBufferLength;\n";
            maxBufferLength_ = subMaxBufferLength;
        }
        iterations_ =  j.maxLength / maxBufferLength_;
        j.iterations =  j.maxLength / maxBufferLength_;

        for (auto i : j.columnList){
            i->setBufferLength(maxBufferLength_ / (j.maxLength / i->getLength()));
        }

    }
    if (groupList.size() !=1)
        print_IR_error("non multiple large array lengths is not supported yet\n");

    //iterations=groupList[0].columnList[0]->getLength()/ groupList[0].columnList[0]->get();
}


void jit_init() {
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
}


//static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

void configOptimization(legacy::FunctionPassManager* TheFPM) {
    TheFPM->add(createPartiallyInlineLibCallsPass());
    TheFPM->add(createPromoteMemoryToRegisterPass());
    TheFPM->add(createInstructionCombiningPass());
    TheFPM->add(createReassociatePass());
    TheFPM->add(createGVNPass());
    TheFPM->add(createCFGSimplificationPass());
    TheFPM->doInitialization();
}

bool Table::llvmInit() {
    context_ = new LLVMContext();

    moduleUPtr  = std::make_unique<Module>("test", *context_);
    theFPM      = std::make_unique<legacy::FunctionPassManager>(moduleUPtr.get());
    M = moduleUPtr.get();

    configOptimization(theFPM.get());
    declareFunctions();

    return true;
}


bool Table::generateIR(std::string basicBlockPrefix) {


    LLVMContext & context = M->getContext();
    if(builder_==NULL)
        builder_=new IRGenerator(context, this);
    IRGenerator &builder= *builder_;

    /*
    auto updateFunction = Function::Create(
        FunctionType::get(Type::getInt32Ty(context), {  }, false),
        Function::ExternalLinkage,
        "updateBuffer",
        M
    );
    */
//auto externalFn_IR = cast<Function> (M->getOrInsertFunction("externalFn",Type::getDoubleTy(context), Type::getDoubleTy(context)));
//Value* x = externalFn_IR->arg_begin();
//x->setName("x");




    mainFunction_ = Function::Create(
        FunctionType::get(Type::getInt32Ty(context), {Type::getInt64PtrTy(context)->getPointerTo()}, false),
        Function::ExternalLinkage,
        "main", 
        M
    );

    Value* bufferUpdateFunction=builder.CreateIntToPtr(
        ConstantInt::get(builder.getInt64Ty(), uintptr_t(updateBuffer)),
        PointerType::getUnqual(
            FunctionType::get(builder.getInt32Ty(),
                { },
                false)));

    bufferUpdateFunction->setName("updateBuffer");


    builder.SetDeclareConvolve(builder.getInt16Ty(),  uintptr_t(convolveTemplate<int16_t>));
    builder.SetDeclareConvolve(builder.getInt32Ty(),  uintptr_t(convolveTemplate<int32_t>));
    builder.SetDeclareConvolve(builder.getInt64Ty(),  uintptr_t(convolveTemplate<int64_t>));
    builder.SetDeclareConvolve(builder.getDoubleTy(), uintptr_t(convolveTemplate<double>));
    builder.SetDeclareConvolve(builder.getFloatTy(),  uintptr_t(convolveTemplate<float>));

    for (auto i : constList)
        i->setupIR(builder);

    for (auto i : smallArrayList)
        i->calculate();


    //i->setupIR(builder);


    ///the main "For loop"
    ///creating jump commands for init cycle
    builder.SetBufferUpdateFunction(bufferUpdateFunction);
    builder.SetCurrentFunction(mainFunction_);
    BasicBlock* bb= BasicBlock::Create(context, "init_block", mainFunction_);
    builder.SetInitInsertPoint(bb);

    for (auto i : columnList_) 
       i->generateIR(builder,CycleStageEn::start);

    ///creating jump commands for init cycle
    builder.CreateStartBRs();
    //cond jump to Main Cycle Enter
    
    BasicBlock* bbUpdateBuffer = BasicBlock::Create(context, "update_buffer_block", builder.getCurrentFunction());
    BasicBlock* bbLoopEnter = BasicBlock::Create(context, "loop_enter_block", builder.getCurrentFunction());

    builder.CreateCondBr(builder.getCurrentCMPRes(), builder.getLoadBlock(), bbUpdateBuffer);

    builder.SetInsertPoint(bbUpdateBuffer);
    builder.CreateCall(bufferUpdateFunction); //call buffer update
    builder.CreateBr(bbLoopEnter);

    builder.SetLoopEnterInsertPoint(bbLoopEnter);
    builder.DropBaseInsertPoint();

    Value* globIndexAlloca=builder.CreatePositionalOffsetAlloca("glob_index_alloca",0);
    builder.CreatePositionalOffsetAlloca("common_offset_alloca", 0);


    builder.SetLoopEnterInsertPoint(bbLoopEnter);
    Value* globIndex=builder.CreateLoad(globIndexAlloca, "glob_index");

    for (auto i : columnList_)
        i->generateIR(builder, CycleStageEn::midle);
   
    builder.CreateMidleBRs();
    BasicBlock* bbCycleExit = BasicBlock::Create(context, "cycle_exit_block", builder.getCurrentFunction());
    builder.CreateCondBr(builder.getCurrentCMPRes(), builder.getLoadBlock(), bbCycleExit);
    builder.SetCycleExitInsertPoint(bbCycleExit);

    ///global index increment, and index comparsion with max iteration number
    ///increasing the global index and comparing the index with the max iteration number
    Value* nextGlobIndex =builder.CreateAdd(globIndex, builder.getInt64(1));
    builder.CreateStore(nextGlobIndex, globIndexAlloca);
    builder.SetCurrentCMPRes(
        builder.CreateICmpULT(
            nextGlobIndex,
            builder.getInt64( (iterations_-1) )));


    BasicBlock* bbTerminalLoopEnter = BasicBlock::Create(context, "terminal_loop_enter", builder.getCurrentFunction());
    builder.CreateCall(bufferUpdateFunction); //call buffer update
    builder.CreateCondBr(builder.getCurrentCMPRes(), bbLoopEnter, bbTerminalLoopEnter);
    builder.SetLoopEnterInsertPoint(bbTerminalLoopEnter);
    builder.DropBaseInsertPoint();
    
    bool  isNotIdle = false;
    for (auto i : columnList_)
        isNotIdle |= i->generateIR(builder, CycleStageEn::end);
   
    if (isNotIdle) {
        builder.CreateMidleBRs();
        BasicBlock* bbExit = BasicBlock::Create(context, "exit_block", builder.getCurrentFunction());
        builder.CreateCondBr(builder.getCurrentCMPRes(), builder.getLoadBlock(), bbExit);
        builder.SetExitInsertPoint(bbExit);
        builder.CreateCall(bufferUpdateFunction); //call buffer update
    }
   


    
    builder.CreateRet(builder.getInt32(1));
    ///create jump from init block to the next
    builder.SetInitInsertPoint();
    builder.CreateBr(builder.getBlock(1));


    //
    g_buffers = builder.getBufferList();
    
    for (auto i : *g_buffers) 
        if(i)
            llvm::outs() << Delimiter::RED << *i;
    

    return true;
}

bool Table::runOptimization() {
    theFPM->run(*mainFunction_);
    return true;
}

bool Table::run() {

    //auto mainF=M->getFunction("main");
    std::string errStr;
    ExecutionEngine* EE = EngineBuilder(std::move(moduleUPtr)) .setErrorStr(&errStr).create();
    if (!EE) {
        llvm::outs() << ": Failed to construct ExecutionEngine: " << errStr << "\n";
        return false;
    }

    if (verifyFunction(*mainFunction_, &llvm::outs())) {
        llvm::outs() << ": Error constructing FooF function!\n\n";
        return false;
    }

    if (verifyModule(*M)) {
        llvm::outs() << ": Error constructing module!\n";
        return false;
    }

    EE->finalizeObject();

    Jit_Call_t Call = (Jit_Call_t)EE->getPointerToFunction(mainFunction_);


    char ** buffers_array = new char*[g_buffers->size()];
    char ** buffers_array_temp=buffers_array;
    for (auto &i : *g_buffers)
        *(buffers_array_temp++)=i->getPtr();

    //llvm::outs()<<buffers <<

    initBuffers();
    Call(buffers_array);

    //updateBuffer();

    for (auto &i : *g_buffers)
        i->close();

    delete [] buffers_array;
    return  true;
}

std::string Table::printllvmIr() {
    llvm::outs() << ExColors::GREEN << "\n\n---------We just constructed this LLVM module:--------- \n" << ExColors::RESET << *mainFunction_ << "\n\n";
    return "";
}


/// Implementation of Value, Operation, Line and Call  members, 
/// which provide llvm IR generation.
///
///
Value* Variable::getIRValue(IRGenerator & builder, int64_t parentLevel) {
    Value * ret=NULL;
    if (isBuffered() & (parentLevel != level_) ) {
        if (!builder.CheckExistence(IRBufferRefPtr_)) {
            auto valPtr=builder.CreatePositionalInBoundsGEP(IRBufferRefPtr_, builder.getCurrentOffsetValue(), "offset_incr");
            IRLoadedBufferValue_=builder.CreatePositionalLoad(valPtr, "buffer_");
            builder.AddInitializedVariable(IRBufferRefPtr_);
        }
        ret=IRLoadedBufferValue_;
    }
    else ret=IRValue_;

    if (ret == NULL) print_IR_error("IRValue - is NULL :" + getUniqueName());
    return ret;
}

Value* Variable::getIRValuePtr(IRGenerator & builder, int64_t parentLevel) {
    auto ret =isBuffered () ? IRBufferRefPtr_ : NULL;
    if (ret == NULL) print_IR_error("getIRValuePtr - is NULL :" + getUniqueName());
    return ret;
}

void  Variable::setupIR(IRGenerator & builder) {
    IRValue_=builder.CreateConst(binaryValue_, type_, "");
}

void  Operation::setupIR(IRGenerator & builder){

#define OP(i)     (operand[(i)]->getAssignedVal(true)->getIRValue(builder,level_) )
#define OP_PTR(i) (operand[(i)]->getAssignedVal(true)->getIRValuePtr(builder,level_) )

    if (isArithetic(opCode)) {
        IRValue_ =builder.CreateArithmetic(OP(0), OP(1), opCode, getUniqueName());
    }
    else if (isInv(opCode)) {
        IRValue_ =builder.CreateInv(OP(0), opCode, getUniqueName());
    }
    else if (isTypeConv(opCode)) {
        IRValue_ =builder.CreateTypeConv(OP(0), opCode, type_, getUniqueName());
    }
    else if (isBuiltInFunc(opCode)) {
        IRValue_ =builder.CreateBuiltInFunc(OP(0), opCode, getUniqueName());
    }
    else if (isSelect(opCode)) {
         
    }
    else if (isConvolve(opCode)) {
        //outs()<<operand[1]->getAssignedVal(true)->printSmallArray()<<"\n";
        IRValue_ =builder.CreateConvolve(OP_PTR(0), OP_PTR(0), getUniqueName());
    }
    else if (isSlice(opCode)) {

    }
    else if (isStoreToBuffer(opCode)) {
        print_IR_error("visitExitTxt StoreToBuffer unknown command .");
    }
    else {
        print_IR_error("visitExitTxt unknown command .");
    }


    if (isBuffered()|isReturned()) {
        if (!is_initialized) {
            BufferTypeEn bufferType=isReturned()? BufferTypeEn::output: BufferTypeEn::internal;
            if (isReturned()) {
                builder.AddBufferAlloca(new OutputBuffer(this));
            }
            else 
                builder.AddBufferAlloca(new Buffer(this));
            
            IRBufferRefPtr_=builder.CreateBufferInit(type_,"internal_");
            is_initialized=true;
        }
        builder.SetStoreInsertPoint();
        auto valPtr=builder.CreateInBoundsGEP(IRBufferRefPtr_, builder.getCurrentOffsetValue(), "offset_incr");
        builder.CreatePositionalStore(IRValue_, valPtr);
    }
    

#undef OP_PTR
#undef OP

}

void  Line::setupIR(IRGenerator & builder) {

    if (!is_arg) {
        //pass
    }
    else {
        //setBuffered();
        Type * volatile t= builder.getLLVMType(type_);
        if (!is_initialized) {
            builder.AddBufferAlloca(new InputBuffer(this));
            IRBufferRefPtr_ = builder.CreateBufferInit(type_, "external_");
            is_initialized  = true;
        }
        auto valPtr    = builder.CreatePositionalInBoundsGEP(IRBufferRefPtr_, builder.getCurrentOffsetValue(), "offset_arg_incr");
        IRValue_       = builder.CreatePositionalLoad(valPtr, "arg_buffer_");
    }
}

void  Call::setupIR(IRGenerator & builder){
    //pass
}