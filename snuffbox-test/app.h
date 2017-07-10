#pragma once

#include <snuffbox-engine/application/application.h>
#include <snuffbox-logging/logging_stream.h>

namespace snuffbox
{
	namespace test
	{
		/**
		* @class snuffbox::test::App : public snuffbox::engine::Application
		* @brief The test application environment
		* @author Daniël Konings
		*/
		class App : public engine::Application
		{
		public:

			/**
			* @brief Default constructor
			*/
			App();

		protected:

			/**
			* @see snuffbox::engine::Application::OnInit
			*/
			void OnInit() override;

			/**
			* @see snuffbox::engine::Application::OnUpdate
			*/
			void OnUpdate() override;

			/**
			* @see snuffbox::engine::Application::OnShutdown
			*/
			void OnShutdown() override;
		};
	}

}