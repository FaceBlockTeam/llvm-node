declare namespace llvm {
  // bitcode/bitcodewriter
  import AttrKind = llvm.Attribute.AttrKind;

  function writeBitcodeToFile(module: Module, filename: string): void;

  // IR/verifier
  function verifyModule(mod: Module): void;

  function verifyFunction(fun: Function): void;

  // support
  function initializeAllTargetInfos(): void;

  function initializeAllTargets(): void;

  function initializeAllTargetMCs(): void;

  function initializeAllAsmParsers(): void;

  function initializeAllAsmPrinters(): void;

  namespace Attribute {
    enum AttrKind {
      Alignment,
      AllocSize,
      AlwaysInline,
      ArgMemOnly,
      Builtin,
      ByVal,
      Cold,
      Convergent,
      Dereferenceable,
      DereferenceableOrNull,
      InAlloca,
      InReg,
      InaccessibleMemOnly,
      InaccessibleMemOrArgMemOnly,
      InlineHint,
      JumpTable,
      MinSize,
      Naked,
      Nest,
      NoAlias,
      NoBuiltin,
      NoCapture,
      NoDuplicate,
      NoImplicitFloat,
      NoInline,
      NoRecurse,
      NoRedZone,
      NoReturn,
      NoUnwind,
      NonLazyBind,
      NonNull,
      OptimizeForSize,
      OptimizeNone,
      ReadNone,
      ReadOnly,
      Returned,
      ReturnsTwice,
      SExt,
      SafeStack,
      SanitizeAddress,
      SanitizeMemory,
      SanitizeThread,
      StackAlignment,
      StackProtect,
      StackProtectReq,
      StackProtectStrong,
      StructRet,
      SwiftError,
      SwiftSelf,
      UWTable,
      WriteOnly,
      ZExt,
    }
  }

  //ir
  enum LinkageTypes {
    ExternalLinkage,
    AvailableExternallyLinkage,
    LinkOnceAnyLinkage,
    LinkOnceODRLinkage,
    WeakAnyLinkage,
    WeakODRLinkage,
    AppendingLinkage,
    InternalLinkage,
    PrivateLinkage,
    ExternalWeakLinkage,
    CommonLinkage,
  }

  enum VisibilityTypes {
    Hidden,
    Default,
    Protected,
  }

  enum AtomicOrdering {
    NotAtomic,
    Unordered,
    Monotonic,
    Acquire,
    Release,
    AcquireRelease,
    SequentiallyConsistent,
  }

  class Value {
    static MaxAlignmentExponent: number;
    static MaximumAlignment: number;

    protected constructor();

    readonly type: Type;
    name: string;

    dump?: () => void;

    hasName(): boolean;

    /**
     * Deletes the value. It is, therefore, forbidden to use the value any further
     */
    release(): void;

    deleteValue(): void;

    replaceAllUsesWith(value: Value): void;

    useEmpty(): boolean;

    getContext(): LLVMContext;
  }

  class Argument extends Value {
    readonly argumentNumber: number;
    readonly parent?: Function;

    constructor(type: Type, name?: string, fn?: Function, argNo?: number);

    addAttr(kind: Attribute.AttrKind): void;

    hasAttribute(kind: Attribute.AttrKind): boolean;

    addDereferenceableAttr(bytes: number): void;
  }

  class LandingPadInst extends Value {
    // TODO: change the type of insertBefore
    static create(type: Type, numReservedClauses: number, name?: string, insertBefore?: Value): LandingPadInst;
    static create(type: Type, numReservedClauses: number, name: string, insertAtEnd: BasicBlock): LandingPadInst;

    getClause(idx: number): Constant;
  }

  class ResumeInst extends Value {
    static create(value: Value, insertAtEnd: BasicBlock): ResumeInst;
  }

  class InvokeInst extends Value {
    getNormalDest(): BasicBlock;
    getUnwindDest(): BasicBlock;
    setNormalDest(b: BasicBlock): void;
    setUnwindDest(b: BasicBlock): void;
    getLandingPadInst(): LandingPadInst;
    getSuccessor(i: number): BasicBlock;
    setSuccessor(i: number, newSucc: BasicBlock): void;
    getNumSuccessors(): number;
  }
  class CatchSwitchInst extends Value {
    static create(parentPad: Value, unwindDest: BasicBlock, numHandlers: number, name?: string, insertBefore?: Value): CatchSwitchInst;
    addHandler(dest: BasicBlock): void;
    getNumHandlers(): number;
    getParentPad(): Value;
    setParentPad(parentPad: Value): void;
    hasUnwindDest(): boolean;
    unwindsToCaller(): boolean;
    getUnwindDest(): BasicBlock;
    setUnwindDest(): void;
  }

  class SwitchInst extends Value {
    static create(value: Value, defaultDest: BasicBlock, numCases: number, insertAtEnd: BasicBlock): SwitchInst;
    getCondition(): Value;
    setCondition(value: Value): void;
    getDefaultDest(): BasicBlock;
    setDefaultDest(defaultDest: BasicBlock): void;
    getNumCases(): number;
    addCase(onVal: ConstantInt, dest: BasicBlock): void;
    getNumSuccessors(): number;
    getSuccessor(idx: number): BasicBlock;
    setSuccessor(idx: number, newSucc: BasicBlock): void;
  }

  class AllocaInst extends Value {
    alignment: number;
    type: PointerType;
    allocatedType: Type;
    readonly arraySize: Value | null;

    private constructor();
  }

  class BasicBlock extends Value {
    static create(context: LLVMContext, name?: string, parent?: Function, insertBefore?: BasicBlock): BasicBlock;

    private constructor();

    readonly parent?: Function;
    readonly empty: boolean;
    readonly context: LLVMContext;

    eraseFromParent(): void;

    getTerminator(): Value | undefined;
  }

  class Constant extends Value {
    static getNullValue(type: Type): Constant;

    static getAllOnesValue(type: Type): Constant;

    protected constructor();

    isNullValue(): boolean;

    isOneValue(): boolean;

    isAllOnesValue(): boolean;
  }

  class ConstantFP extends Constant {
    static get(context: LLVMContext, value: number): ConstantFP;
    static get(type: Type, value: string): Constant;

    static getZeroValueForNegation(type: Type): Constant;

    static getNegativeZero(type: Type): Constant;

    static getNaN(type: Type): Constant;

    static getInfinity(type: Type, negative?: boolean /* = false */): Constant;

    private constructor();

    readonly value: number;
  }

  class ConstantInt extends Constant {
    static get(context: LLVMContext, value: number | string, numBits?: number, signed?: boolean): ConstantInt;

    static getFalse(context: LLVMContext): ConstantInt;

    static getTrue(context: LLVMContext): ConstantInt;

    private constructor();

    readonly value: number;

    public toString(): string;
  }

  class ConstantPointerNull extends Constant {
    static get(pointerType: PointerType): ConstantPointerNull;

    private constructor();
  }

  class ConstantArray extends Constant {
    static get(arrayType: ArrayType, elements: Constant[]): Constant;
  }

  class ConstantDataArray extends Constant {
    static get(llvmContext: LLVMContext, values: Uint32Array | Float32Array | Float64Array): Constant;

    static getString(llvmContext: LLVMContext, value: string): Constant;

    private constructor();
  }

  class ConstantStruct extends Constant {
    static get(structType: StructType, values: Constant[]): Constant;

    private constructor();
  }

  class DILocalScope extends DIScope {
    getSubprogram(): DISubprogram;
  }

  class DILocation {
    private constructor();
    static get(context: LLVMContext, line: number, column: number, scope: DILocalScope): DILocation;
  }

  class DIFile extends DIScope {
    private constructor();
  }

  class DICompileUnit extends DIScope {
    private constructor();
  }

  class DIDerivedType extends DIType {
    private constructor();
  }

  class DIBasicType extends DIType {
    private constructor();
  }

  class DICompositeType extends DIType {
    private constructor();
  }

  class DIStringType extends DIType {
    private constructor();
  }

  class DIType extends DIScope {
    getLine(): number;
    getScope(): DIScope;
    getName(): string;
  }

  class DIFlags {
    private constructor();
  }

  class DIExpression {
    private constructor();
  }

  class DISubprogram extends DILocalScope {
    private constructor();
  }

  class DIModule extends DIScope {
    private constructor();
  }

  class DIScope extends DINode {
    getFile(): DIFile;
    getFilename(): string;
    getDirectory(): string;
    getName(): string;
    getContext(): LLVMContext;
  }

  class DINode extends Metadata {
    protected constructor();
  }

  namespace DINode {
    enum DIFlags {
      FlagAccessibility,
      FlagAllCallsDescribed,
      FlagAppleBlock,
      FlagArtificial,
      FlagBigEndian,
      FlagBitField,
      FlagEnumClass,
      FlagExplicit,
      FlagExportSymbols,
      FlagFwdDecl,
      FlagIndirectVirtualBase,
      FlagIntroducedVirtual,
      FlagLargest,
      FlagLittleEndian,
      FlagLValueReference,
      FlagMultipleInheritance,
      FlagNonTrivial,
      FlagNoReturn,
      FlagObjcClassComplete,
      FlagObjectPointer,
      FlagPrivate,
      FlagProtected,
      FlagPrototyped,
      FlagPtrToMemberRep,
      FlagPublic,
      FlagReservedBit4,
      FlagRValueReference,
      FlagSingleInheritance,
      FlagStaticMember,
      FlagThunk,
      FlagTypePassByReference,
      FlagTypePassByValue,
      FlagVector,
      FlagVirtual,
      FlagVirtualInheritance,
      FlagZero
    }
  }

  namespace Metadata {

    enum StorageType {
      Uniqued,
      Distinct,
      Temporary
    }
  }

  class Metadata {
    constructor(id: number, storage: Metadata.StorageType);
    dump(): void;
    getMetadataID(): number;
  }

  class DISubroutineType extends DIType {
    private constructor();
  }

  class DIVariable {

  }

  class DILocalVariable extends DIVariable {

  }

  class Instruction extends Value {
    getParent(): BasicBlock;
    getFunction(): Function;
    getModule(): Module | undefined;
    getNumSuccessors(): number;
  }

  class DIBuilder {
    constructor(module: Module, allowUnresolved?: boolean, cu?: DICompileUnit);
    insertDeclare(storage: Value, varInfo: DILocalVariable, expr: DIExpression, dl: DILocation, insertAtEnd: BasicBlock): Instruction;
    insertDbgValueIntrinsic(val: Value, varInfo: DILocalVariable, expr: DIExpression, dl: DILocation, insertAtEnd: BasicBlock): Instruction;
    getOrCreateTypeArray(elements: Metadata[]): DITypeRefArray;
    getOrCreateArray(elements: Metadata[]): DINodeArray;
    createUnspecifiedType(name: string): DIBasicType;
    createSubroutineType(parameterTypes: DITypeRefArray): DISubroutineType;
    createBasicType(name: string, sizeInBits: number, encoding: number): DIBasicType;
    createStructType(scope: DIScope, name: string, file: DIFile, lineNo: number, sizeInBits: number, alignInBits: number, flags: DIFlags, derivedFrom: DIType, elements: DINodeArray): DICompositeType;
    createAutoVariable(scope: DIScope, name: string, file: DIFile, lineNo: number, type: DIType): DILocalVariable;
    createParameterVariable(scope: DIScope, name: string, argNo: number, file: DIFile, lineNo: number, type: DIType): DILocalVariable;
    createCompileUnit(lang: number, file: DIFile, producer: string, isOptimized: boolean, flags: string, rv: number): DICompileUnit;
    createFile(filename: string, directory: string): DIFile;
    createInheritance(type: DIType, baseType: DIType, baseOffset: number, vBPtrOffset: number, flags: DIFlags): DIDerivedType;
    createExpression(addr?: number[]): DIExpression;
    createFunction(scope: DIScope, name: string, linkageName: string, file: DIFile, lineNo: number, type: DISubroutineType, scopeLine: number): DISubprogram;
    createModule(scope: DIScope, name: string, configurationMacros: string, includePath: string): DIModule;
    finalize(): void;
    finalizeSubprogram(diSubprogram: DISubprogram): void;
  }

  type DITypeRefArray = Array<DIType>;

  type DINodeArray = Array<DINode>;

  enum dwarf {
    DW_ATE_address = 0x01,
    DW_ATE_boolean = 0x02,
    DW_ATE_complex_float = 0x03,
    DW_ATE_float = 0x04,
    DW_ATE_signed = 0x05,
    DW_ATE_signed_char = 0x06,
    DW_ATE_unsigned = 0x07,
    DW_ATE_unsigned_char = 0x08
  }

  class Function extends Constant {
    static create(functionType: FunctionType, linkageTypes: LinkageTypes, name?: string, module?: Module): Function;
    static lookupIntrinsicID(name: string): Intrinsic.ID;
    callingConv: CallingConv;
    visibility: VisibilityTypes;
    type: PointerType & { elementType: FunctionType };

    private constructor();

    addAttribute(index: number, attribute: Attribute.AttrKind): void;

    addBasicBlock(basicBlock: BasicBlock): void;

    addDereferenceableAttr(attributeIndex: number, bytes: number): void;

    addDereferenceableOrNullAttr(attributeIndex: number, bytes: number): void;

    addFnAttr(attribute: Attribute.AttrKind): void;

    getArguments(): Argument[];

    getEntryBlock(): BasicBlock | null;

    getBasicBlocks(): BasicBlock[];

    viewCFG(): void;

    setSubprogram(subprogram: DISubprogram): void;

    getSubprogram(): DISubprogram;
  }

  class GlobalVariable extends Constant {
    constant: boolean;
    initializer: Constant | undefined;

    setUnnamedAddr(unnamedAddr: UnnamedAddr): void;

    hasGlobalUnnamedAddr(): boolean;

    constructor(
      module: Module,
      type: Type,
      constant: boolean,
      linkageType: LinkageTypes,
      initializer?: Constant,
      name?: string
    );
  }

  enum UnnamedAddr {
    None,
    Local,
    Global,
  }

  class PhiNode extends Value {
    private constructor();

    readonly elementType: Type;

    addIncoming(value: Value, basicBlock: BasicBlock): void;
  }

  class CallInst extends Value {
    callingConv: CallingConv;

    private constructor();

    addDereferenceableAttr(index: number, size: number): void;

    hasRetAttr(kind: AttrKind): boolean;

    paramHasAttr(index: number, kind: AttrKind): boolean;

    getNumArgOperands(): number;
  }

  enum CallingConv {
    C,
    Fast,
    Cold,
    GHC,
    HiPE,
    WebKit_JS,
    AnyReg,
    PreserveMost,
    PreserveAll,
    CXX_FAST_TLS,
    FirstTargetCC,
    X86_StdCall,
    X86_FastCall,
    ARM_APCS,
    ARM_AAPCS,
    ARM_AAPCS_VFP,
    MSP430_INTR,
    X86_ThisCall,
    PTX_Kernel,
    PTX_Device,
    SPIR_FUNC,
    SPIR_KERNEL,
    Intel_OCL_BI,
    X86_64_SysV,
    Win64,
    X86_VectorCall,
    HHVM,
    HHVM_C,
    X86_INTR,
    AVR_INTR,
    AVR_SIGNAL,
    AVR_BUILTIN,
    AMDGPU_VS,
    AMDGPU_GS,
    AMDGPU_PS,
    AMDGPU_CS,
    AMDGPU_KERNEL,
    X86_RegCall,
    AMDGPU_HS,
    MSP430_BUILTIN,
    MaxID,
  }

  class UndefValue {
    private constructor();

    static get(type: Type): UndefValue;
  }

  class DataLayout {
    constructor(layout: string);

    getStringRepresentation(): string;

    getPointerSize(as: number): number;

    getPrefTypeAlignment(type: Type): number;

    getTypeStoreSize(type: Type): number;

    getIntPtrType(context: LLVMContext, as: number): Type;
  }

  class Type {
    static TypeID: {
      VoidTyID: number;
      HalfTyID: number;
      FloatTyID: number;
      DoubleTyID: number;
      X86_FP80TyID: number;
      PPC_FP128TyID: number;
      LabelTyID: number;
      MetadataTyID: number;
      X86_MMXTyID: number;
      TokenTyID: number;
      IntegerTyID: number;
      FunctionTyID: number;
      StructTyID: number;
      ArrayTyID: number;
      PointerTyID: number;

      // LLVM 11+
      FixedVectorTyID: number;
      ScalableVectorTyID: number;

      /**
       * @deprecated
       */
      VectorTyID: number;
    };

    static getFloatTy(context: LLVMContext): Type;

    static getDoubleTy(context: LLVMContext): Type;

    static getFP128Ty(context: LLVMContext): Type;

    static getVoidTy(context: LLVMContext): Type;

    static getLabelTy(context: LLVMContext): Type;

    static getInt1Ty(context: LLVMContext): IntegerType;

    static getInt8Ty(context: LLVMContext): IntegerType;

    static getInt16Ty(context: LLVMContext): IntegerType;

    static getInt32Ty(context: LLVMContext): IntegerType;

    static getInt64Ty(context: LLVMContext): IntegerType;

    static getInt128Ty(context: LLVMContext): IntegerType;

    static getIntNTy(context: LLVMContext, N: number): IntegerType;

    static getInt1PtrTy(context: LLVMContext, AS?: number): PointerType;

    static getInt8PtrTy(context: LLVMContext, AS?: number): PointerType;

    static getInt32PtrTy(context: LLVMContext, AS?: number): PointerType;

    static getDoublePtrTy(context: LLVMContext, AS?: number): PointerType;

    static getFloatPtrTy(context: LLVMContext, AS?: number): PointerType;

    static getHalfTy(context: LLVMContext): Type;

    protected constructor();

    readonly typeID: number;

    equals(other: Type): boolean;

    isVoidTy(): boolean;

    isFloatTy(): boolean;

    isDoubleTy(): boolean;

    isFP128Ty(): boolean;

    isLabelTy(): boolean;

    isIntegerTy(bitWidth?: number): boolean;

    isFunctionTy(): this is FunctionType;

    isStructTy(): this is StructType;

    isArrayTy(): boolean;

    isHalfTy(): boolean;

    isPointerTy(): this is PointerType;

    getPointerTo(addressSpace?: number): PointerType;

    getPrimitiveSizeInBits(): number;

    toString(): string;
  }

  class IntegerType extends Type {
    private constructor();

    getBitWidth(): number;
  }

  class FunctionType extends Type {
    static get(result: Type, isVarArg: boolean): FunctionType;
    static get(result: Type, params: Type[], isVarArg: boolean): FunctionType;

    static isValidReturnType(type: Type): boolean;

    static isValidArgumentType(type: Type): boolean;

    readonly returnType: Type;
    readonly isVarArg: boolean;
    readonly numParams: number;

    private constructor();

    getParams(): Type[];

    getParamType(index: number): Type;
  }

  class PointerType extends Type {
    static get(elementType: Type, AS: number): PointerType;
    static get(context: LLVMContext, AS: number): PointerType;
    elementType: Type;

    private constructor();
  }

  class ArrayType extends Type {
    static get(elementType: Type, numElements: number): ArrayType;

    readonly elementType: Type;
    readonly numElements: number;

    private constructor();
  }

  class StructType extends Type {
    static create(context: LLVMContext, name?: string): StructType;

    static get(context: LLVMContext, elements: Type[], isPacked?: boolean): StructType;

    name: string | undefined;
    readonly numElements: number;

    private constructor();

    getElementType(index: number): Type;

    setBody(elements: Type[], packed?: boolean): void;
  }

  class IRBuilder {
    constructor(context: LLVMContext);
    constructor(basicBlock: BasicBlock, beforeInstruction?: Value);

    setInsertionPoint(basicBlock: BasicBlock): void;

    createAdd(lhs: Value, rhs: Value, name?: string): Value;

    createAlloca(type: Type, arraySize?: Value, name?: string): AllocaInst;

    createAlignedLoad(type: Type, ptr: Value, align: number, name?: string): Value;

    createAlignedStore(value: Value, ptr: Value, align: number, isVolatile?: boolean): Value;

    createAnd(lhs: Value, rhs: Value, name?: string): Value;

    createAShr(lhs: Value, rhs: Value, name?: string): Value;

    createAtomicRMW(op: AtomicRMWInst.BinOp, ptr: Value, value: Value, align: number, ordering: AtomicOrdering): Value;

    createBitCast(value: Value, destType: Type, name?: string): Value;

    createBr(basicBlock: BasicBlock): Value;

    /**
     * @deprecated
     */
    createCall(callee: Value, args: Value[], name?: string): CallInst;

    createCall(functionType: FunctionType, callee: Value, args: Value[], name?: string): CallInst;

    createCondBr(condition: Value, then: BasicBlock, elseBlock: BasicBlock): Value;

    createExtractValue(agg: Value, idxs: number[], name?: string): Value;

    createFAdd(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpOGE(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpOGT(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpOLE(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpOLT(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpOEQ(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpONE(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpUGE(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpUGT(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpULE(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpULT(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpUEQ(lhs: Value, rhs: Value, name?: string): Value;

    createFCmpUNE(lhs: Value, rhs: Value, name?: string): Value;

    createFDiv(lhs: Value, rhs: Value, name?: string): Value;

    createFNeg(value: Value, name?: string): Value;

    createFMul(lhs: Value, rhs: Value, name?: string): Value;

    createFRem(lhs: Value, rhs: Value, name?: string): Value;

    createFSub(lhs: Value, rhs: Value, name?: string): Value;

    createFPToSI(value: Value, type: Type, name?: string): Value;

    createGlobalString(str: string, name?: string, addressSpace?: number): Value;

    createGlobalStringPtr(str: string, name?: string, addressSpace?: number): Value;

    createInBoundsGEP(ptr: Value, idxList: Value[], name?: string): Value;
    createInBoundsGEP(type: Type, ptr: Value, idxList: Value[], name?: string): Value;

    createInsertValue(agg: Value, val: Value, idxList: number[], name?: string): Value;

    createInvoke(type: FunctionType, callee: Value, normalDest: BasicBlock, unwindDest: BasicBlock, args: Value[], name?: string): Value;

    createIntCast(vlaue: Value, type: Type, isSigned: boolean, name?: string): Value;

    createICmpEQ(lhs: Value, rhs: Value, name?: string): Value;

    createICmpNE(lhs: Value, rhs: Value, name?: string): Value;

    createICmpSGE(lhs: Value, rhs: Value, name?: string): Value;

    createICmpSGT(lhs: Value, rhs: Value, name?: string): Value;

    createICmpSLE(lhs: Value, rhs: Value, name?: string): Value;

    createICmpSLT(lhs: Value, rhs: Value, name?: string): Value;

    createICmpUGE(lhs: Value, rhs: Value, name?: string): Value;

    createICmpUGT(lhs: Value, rhs: Value, name?: string): Value;

    createICmpULE(lhs: Value, rhs: Value, name?: string): Value;

    createICmpULT(lhs: Value, rhs: Value, name?: string): Value;

    createLandingPad(type: Type, numClauses: number, name?: string): LandingPadInst;

    createLoad(ptr: Value, name?: string): Value;

    createLShr(lhs: Value, rhs: Value, name?: string): Value;

    createMul(lhs: Value, rhs: Value, name?: string): Value;

    createNeg(value: Value, name?: string, hasNUW?: boolean, hasNSW?: boolean): Value;

    createNot(value: Value, name?: string): Value;

    createOr(lhs: Value, rhs: Value, name?: string): Value;

    createXor(lhs: Value, rhs: Value, name?: string): Value;

    createPhi(type: Type, numReservedValues: number, name?: string): PhiNode;

    createPtrToInt(value: Value, destType: Type, name?: string): Value;

    createIntToPtr(value: Value, destType: Type, name?: string): Value;

    createRet(value: Value): Value;

    createResume(value: Value): Value;

    createRetVoid(): Value;

    createSelect(condition: Value, trueValue: Value, falseValue: Value, name?: string): Value;

    createSDiv(lhs: Value, rhs: Value, name?: string): Value;

    createShl(lhs: Value, rhs: Value, name?: string): Value;

    createSwitch(value: Value, dest: BasicBlock, numCases: number): SwitchInst;

    createSIToFP(value: Value, type: Type, name?: string): Value;

    createUIToFP(value: Value, type: Type, name?: string): Value;

    createSub(lhs: Value, rhs: Value, name?: string): Value;

    createStore(value: Value, ptr: Value, isVolatile?: boolean): Value;

    createSRem(lhs: Value, rhs: Value, name?: string): Value;

    CreateURem(lhs: Value, rhs: Value, name?: string): Value;

    createZExt(value: Value, destType: Type, name?: string): Value;

    getInsertBlock(): BasicBlock | undefined;

    setCurrentDebugLocation(dl: DILocation): void;
  }

  namespace Intrinsic {
    type ID = number;
    function getName(id: ID): string;
    function getType(context: LLVMContext, id: ID, types?: Type[]): FunctionType;
    function getDeclaration(module: Module, id: ID, types?: Type[]): Function;
  }

  namespace AtomicRMWInst {
    enum BinOp {
      Add,
      Sub,
      And,
      Nand,
      Or,
      Xor,
      Max,
      Min,
      UMax,
      UMin,
    }
  }

  class LLVMContext {
    constructor();

    // any property that makes ts emits an error if any other object is passed to a method that is not an llvm context
    private __marker: number;
  }

  interface FunctionCallee {
    callee: Value;
    functionType: FunctionType;
  }

  class Module {
    empty: boolean;
    readonly name: string;
    moduleIdentifier: string;
    sourceFileName: string;
    dataLayout: DataLayout;
    targetTriple: string;

    constructor(moduleId: string, context: LLVMContext);

    dump?: () => void;

    print(): string;

    getFunction(name: string): Function | undefined;

    getOrInsertFunction(name: string, functionType: FunctionType): FunctionCallee;

    getGlobalVariable(name: string, allowInternal?: boolean): GlobalVariable;

    getTypeByName(name: string): StructType | null;
  }

  // support
  class TargetRegistry {
    private constructor();

    static lookupTarget(target: string): Target;
  }

  interface Target {
    readonly name: string;
    readonly shortDescription: string;
    createTargetMachine(triple: string, cpu: string): TargetMachine;
  }

  interface TargetMachine {
    createDataLayout(): DataLayout;
  }

  export const config: Readonly<{
    LLVM_VERSION_MAJOR: number;
    LLVM_VERSION_MINOR: number;
    LLVM_VERSION_PATCH: number;
    LLVM_VERSION_STRING: string;
    LLVM_DEFAULT_TARGET_TRIPLE: string;
  }>;
}

export = llvm;
export as namespace llvm;
