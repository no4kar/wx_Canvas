#ifndef MY_MACROS_H_
#define MY_MACROS_H_


#define FRAME_CLASS_NAME CnvsFrame
#define APP_CLASS_NAME CnvsApp

#define CANCAT2(t1, t2) t1##t2
#define CANCAT3(t1, t2, t3) CANCAT2(t1, t2)##t3
#define MY_DEBUG 1
#define ASK_ACT(ask,act) if(##ask){##act##;}
#define m(x) m_##x      /*member*/
#define m_p(x) m(p_##x)     /*member pointer*/
#define CLASS_CONSTRUCT(class_name) class_name
#define CLASS_DESTRUCT(class_name) virtual ~class_name

#if MY_DEBUG && _DEBUG
#define CRTE(x) new x
#define DSTR(x) delete x
#define MY_TRY(x) try{
#define MY_CATCH(x) } catch (const std::exception& ex) { wxMessageBox(ex.what()); }
#else
#define CRTE(x)
#define DSTR(x)
#define MY_TRY(x)
#define MY_CATCH(x)
#endif/*TO_HISTORY*/

typedef const char* cstr_t;

#endif // !MY_MACROS_H_