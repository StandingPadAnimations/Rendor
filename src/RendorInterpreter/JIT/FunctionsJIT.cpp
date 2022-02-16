#include "RendorInterpreter/JIT/RendorJit.hpp"

void RendorJIT::CreateFunction()
{
    m_RendorJITCodeHolder.attach(&m_RendorJITCompiler);
    m_RendorJITCompiler.addFunc(asmjit::FuncSignatureT<void>());
}

void RendorJIT::EndFunction()
{
    m_RendorJITCompiler.endFunc();
    m_RendorJITCompiler.finalize();

    /* ------------------------ Check if code is compiled ----------------------- */
    if (m_RendorJITCompiler.isFinalized())
    {
        RendorFunc* RendorJITFunc;
        asmjit::Error RendorJITAddFunc = m_RendorJITRuntime.add(&RendorJITFunc, &m_RendorJITCodeHolder);

        /* ------------- Check if there's an error when adding function ------------- */
        if (RendorJITAddFunc)
        {
            throw error::RendorException("RendorJIT function adding error: to avoid this please disable JIT");
        }
        
        /* -------------------------- reset the codeholder -------------------------- */
        m_RendorJITFunctions.push_back(RendorJITFunc);
        m_RendorJITCodeHolder.reset();
    }
    else 
    {
        throw error::RendorException("RendorJIT finalize error");
    }
}

