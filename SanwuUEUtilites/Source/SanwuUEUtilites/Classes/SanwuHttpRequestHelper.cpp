#include "SanwuUEUtilitesPrivatePCH.h"
#include "SanwuHttpRequestHelper.h"
#include "Http.h"

DEFINE_LOG_CATEGORY_STATIC(LogHttpRequest, Warning, All);


USanwuHttpRequestHelper::USanwuHttpRequestHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USanwuHttpRequestHelper* USanwuHttpRequestHelper::HttpRequestString(FString url, SanwuHttpRequestType Type, FString Content)
{
	USanwuHttpRequestHelper* Helper = NewObject<USanwuHttpRequestHelper>();
	Helper->Start(url,Type,Content);
	return Helper;
}
void USanwuHttpRequestHelper::Start(FString url, SanwuHttpRequestType Type, FString Content)
{
	TSharedRef<class IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &USanwuHttpRequestHelper::HandleRequestComplete);
	HttpRequest->SetURL(url);
	if (Type==SanwuHttpRequestType::GET)
	{
		HttpRequest->SetVerb(TEXT("GET"));
	}
	else {
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("text/html; charset=utf-8"));
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContentAsString(Content);
	}
	HttpRequest->ProcessRequest();
}
void USanwuHttpRequestHelper::HandleRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded)
	{
		OnSuccess.Broadcast(HttpResponse->GetContentAsString(), HttpResponse->GetResponseCode());
	}
	else
	{
		OnFail.Broadcast("", HttpResponse->GetResponseCode());
	}
}