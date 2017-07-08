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

			/**
			* @brief Converts a HSL value to RGB
			* @param[in] h (float) The hue value
			* @param[in] s (float) The saturation value
			* @param[in] l (float) The lightness value
			* @param[out] converted (unsigned char*) The converted value in the 0 < 255 range
			*/
			static void HSLToRGB(float h, float s, float l, unsigned char* converted);

		private:

			float offset_; //!< The hue offset for the HSL logging
		};
	}

}