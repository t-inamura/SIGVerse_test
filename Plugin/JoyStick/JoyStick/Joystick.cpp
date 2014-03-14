#include "SIGService.h"
#include <boost/python.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <string>


namespace py = boost::python;  
using namespace std; 




std::string parse_python_exception(); // functional declaration for exception handling

template <typename T> string tostr(const T& t) { ostringstream os; os<<t; return os.str(); } // template to convert double variables to string



class JoyStick_Service : public sigverse::SIGService  
  {  
public:  
    JoyStick_Service(std::string name);  
    ~JoyStick_Service();  
    double onAction();  
    void onRecvMsg(sigverse::RecvMsgEvent &evt);  
 };  
  JoyStick_Service::JoyStick_Service(std::string name) : SIGService(name){  
    };  
JoyStick_Service::~JoyStick_Service()  
 {  
    this->disconnect();  
 } 
void JoyStick_Service::onRecvMsg(sigverse::RecvMsgEvent &evt)  
{  
}

double JoyStick_Service::onAction()  
{  

	Py_Initialize();
     
	static int count=1;

	try{
		py::object main_module = py::import("__main__");  
		py::object main_namespace = main_module.attr("__dict__");
		
		if (count == 1){
			py::exec("import Joystick as joy", main_namespace);
			py::exec("textPrint=joy.TextPrint()", main_namespace);
			py::exec("a=textPrint.joystick()", main_namespace);
		}
	
		float dataArray[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		std::string msgFromJoy("");
		char tmp[1024];

		py::object listOfPos = py::eval("next(a)", main_namespace);

	    int lenList=len((listOfPos));

		if (lenList==16){
			for(int i=0; i<lenList; i++){
				dataArray[i] = static_cast<float>(py::extract<double>((listOfPos)[i]));	
			}
			sprintf_s(tmp,  "JOYSTICK_DATA,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4], dataArray[5], dataArray[6], dataArray[7], dataArray[8], dataArray[9], dataArray[10], dataArray[11], dataArray[12], dataArray[13], dataArray[14], dataArray[15]);
			msgFromJoy = std::string(tmp);

			this->sendMsgToCtr("man_000", msgFromJoy);
		}
	}

    catch(boost::python::error_already_set const &){
        // Parse and output the exception
        std::string perror_str = parse_python_exception();
    }

  count ++ ;
  Sleep(20);
  return 0.0;

}

int main(int argc, char** argv)
{
	// Create an instance of the service class with the specified service name
	JoyStick_Service srv("Throwing_Service");
	std::string host = argv[1];
	unsigned short port = (unsigned short)(atoi(argv[2]));  
    srv.connect(host, port); 
	srv.startLoop();

	return 0;
}




std::string parse_python_exception(){
    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    // Fetch the exception info from the Python C API
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

    // Fallback error
    std::string ret("Unfetchable Python error");
    // If the fetch got a type pointer, parse the type into the exception string
    if(type_ptr != NULL){
        py::handle<> h_type(type_ptr);
        py::str type_pstr(h_type);
        // Extract the string from the boost::python object
        py::extract<std::string> e_type_pstr(type_pstr);
        // If a valid string extraction is available, use it 
        //  otherwise use fallback
        if(e_type_pstr.check())
            ret = e_type_pstr();
        else
            ret = "Unknown exception type";
    }
    // Do the same for the exception value (the stringification of the exception)
    if(value_ptr != NULL){
        py::handle<> h_val(value_ptr);
        py::str a(h_val);
        py::extract<std::string> returned(a);
        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }
    // Parse lines from the traceback using the Python traceback module
    if(traceback_ptr != NULL){
        py::handle<> h_tb(traceback_ptr);
        // Load the traceback module and the format_tb function
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        // Call format_tb to get a list of traceback strings
        py::object tb_list(fmt_tb(h_tb));
        // Join the traceback strings into a single string
        py::object tb_str(py::str("\n").join(tb_list));
        // Extract the string, check the extraction, and fallback in necessary
        py::extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}
