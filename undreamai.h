#pragma once
#include "server.cpp"

class StringWrapper {
    private:
        char *content = nullptr;

    public:
        StringWrapper();
        void SetContent(std::string input);
        int GetStringSize();
        void GetString(char* buffer, int bufferSize);
};

typedef void (*CompletionCallback)();
class StringWrapperCallback{
    public:
        StringWrapperCallback(StringWrapper* stringWrapper_in, CompletionCallback callback_in) : stringWrapper(stringWrapper_in), callback(callback_in){}
        void Call(std::string content)
        {
            if (callback == nullptr) return;
            if (stringWrapper != nullptr) stringWrapper->SetContent(content);
            callback();
        }
    private:
        StringWrapper* stringWrapper = nullptr;
        CompletionCallback callback = nullptr;
};

class LLM {
    public:
        LLM(std::string params_string);
        LLM(int argc, char ** argv);
        ~LLM();

        static std::vector<std::string> splitArguments(const std::string& inputString);
        
        std::string handle_tokenize(json body);
        std::string handle_detokenize(json body);
        std::string handle_completions(json data, StringWrapperCallback* callback=nullptr);
        void handle_slots_action(json data);

    private:
        gpt_params params;
        server_params sparams;
        server_context ctx_server;
        std::thread server_thread;

        void init(int argc, char ** argv);
        void parse_args(std::string params_string);
        void run_server();
};

#ifdef _WIN32
    #ifdef UNDREAMAI_EXPORTS
        #define UNDREAMAI_API __declspec(dllexport)
    #else
        #define UNDREAMAI_API __declspec(dllimport)
    #endif
#else
    #define UNDREAMAI_API
#endif

extern "C" {
	UNDREAMAI_API StringWrapper* StringWrapper_Construct();
	UNDREAMAI_API void StringWrapper_Delete(StringWrapper* object);
	UNDREAMAI_API int StringWrapper_GetStringSize(StringWrapper* object);
	UNDREAMAI_API void StringWrapper_GetString(StringWrapper* object, char* buffer, int bufferSize);

    UNDREAMAI_API LLM* LLM_Construct(const char* params_string);
    UNDREAMAI_API void LLM_Delete(LLM* llm);
    UNDREAMAI_API const void LLM_Tokenize(LLM* llm, const char* json_data, StringWrapper* wrapper);
    UNDREAMAI_API const void LLM_Detokenize(LLM* llm, const char* json_data, StringWrapper* wrapper);
    UNDREAMAI_API void LLM_Completion(LLM* llm, const char* json_data, StringWrapper* wrapper, void* streamCallbackPointer=nullptr);
    UNDREAMAI_API const void LLM_Slot(LLM* llm, const char* json_data);
};