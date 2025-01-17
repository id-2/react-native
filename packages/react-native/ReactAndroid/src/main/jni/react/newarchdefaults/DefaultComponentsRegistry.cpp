/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultComponentsRegistry.h"

#include <CoreComponentsRegistry.h>
#include <fbjni/fbjni.h>
#include <react/debug/react_native_assert.h>
#include <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>
#include <react/renderer/components/rncore/ComponentDescriptors.h>

namespace facebook::react {

std::function<void(std::shared_ptr<const ComponentDescriptorProviderRegistry>)>
    DefaultComponentsRegistry::registerComponentDescriptorsFromEntryPoint{};

DefaultComponentsRegistry::DefaultComponentsRegistry(ComponentFactory* delegate)
    : delegate_(delegate) {}

std::shared_ptr<const ComponentDescriptorProviderRegistry>
DefaultComponentsRegistry::sharedProviderRegistry() {
  auto providerRegistry = CoreComponentsRegistry::sharedProviderRegistry();

  react_native_assert(
      DefaultComponentsRegistry::registerComponentDescriptorsFromEntryPoint &&
      "'registerComponentDescriptorsFromEntryPoint' was not initialized in 'JNI_OnLoad'");

  (DefaultComponentsRegistry::registerComponentDescriptorsFromEntryPoint)(
      providerRegistry);

  return providerRegistry;
}

jni::local_ref<DefaultComponentsRegistry::jhybriddata>
DefaultComponentsRegistry::initHybrid(
    jni::alias_ref<jclass>,
    ComponentFactory* delegate) {
  auto instance = makeCxxInstance(delegate);

  auto buildRegistryFunction =
      [](const EventDispatcher::Weak& eventDispatcher,
         const ContextContainer::Shared& contextContainer)
      -> ComponentDescriptorRegistry::Shared {
    ComponentDescriptorParameters params{
        .eventDispatcher = eventDispatcher,
        .contextContainer = contextContainer,
        .flavor = nullptr};

    auto registry = DefaultComponentsRegistry::sharedProviderRegistry()
                        ->createComponentDescriptorRegistry(params);

    auto& mutableRegistry = const_cast<ComponentDescriptorRegistry&>(*registry);
    mutableRegistry.setFallbackComponentDescriptor(
        std::make_shared<UnimplementedNativeViewComponentDescriptor>(params));

    return registry;
  };

  delegate->buildRegistryFunction = buildRegistryFunction;
  return instance;
}

void DefaultComponentsRegistry::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", DefaultComponentsRegistry::initHybrid),
  });
}

} // namespace facebook::react
