/**
 * @file RenderGraph.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <SavannaEngine.h>


typedef enum se_GpuNodeType_t
{
    se_GpuNodeType_Invalid = 0,

    se_GpuNodeType_RenderPass,
    se_GpuNodeType_ComputePass,
    se_GpuNodeType_Transfer,
    se_GpuNodeType_Readback,

    se_GpuNodeType_Count // Must always be last
} se_GpuNodeType;

namespace Savanna::Gfx
{
    using GpuNodeType = se_GpuNodeType_t;

    /**
     * @brief A common interface for all nodes that can be used in a render graph
     * these nodes are intended to generate a DAG of dependencies that can be used to
     * determine the order in which to execute the nodes. Examples of possible nodes
     * include:
     *
     * - Render Passes
     * - Compute Passes
     * - Transfers (CPU -> GPU, GPU -> CPU) or (GPU Render Resource -> GPU Compute Resource)
     *
     * The render graph will be responsible for generating the dependency graph but work submitted
     * will come from the application.
     *
     */
    class IGpuNode
    {
    public:
        IGpuNode() = delete;
        IGpuNode(const GpuNodeType type) : m_Type(type) {}
        virtual ~IGpuNode() = default;

        virtual void Setup(IGpuNode** inputs, IGpuNode** outputs) = 0;
        virtual void Execute() = 0;

        const GpuNodeType GetType() const { return m_Type; }

    protected:
        const GpuNodeType m_Type;
        dynamic_array<IGpuNode*> m_InputNodes;
        dynamic_array<IGpuNode*> m_OutputNodes;
    };

    class GpuProgramPassNode : public IGpuNode
    {
    public:
        GpuProgramPassNode() = delete;
        GpuProgramPassNode(const GpuNodeType type) : IGpuNode(type) {}
        ~GpuProgramPassNode() = default;

    protected:
        GfxProgramHandle m_ProgramHandle;
        GfxBufferHandle* m_pUniformBuffers;
        GfxBufferHandle* m_pStorageBuffers;
        GfxTextureHandle* m_pTextures;
        GfxSamplerHandle* m_pSamplers;

    };

    class RenderPassNode : public GpuProgramPassNode
    {
    public:
        RenderPassNode() : GpuProgramPassNode(GpuNodeType::se_GpuNodeType_RenderPass) {}
        ~RenderPassNode() = default;

        void Setup(IGpuNode** inputs, IGpuNode** outputs) override
        {
            // TODO @DavidMohrhardt: Implement
        }

        void Execute() override
        {
            // TODO @DavidMohrhardt: Implement
        }

    private:
    };

    class ComputePassNode : public GpuProgramPassNode
    {
        ComputePassNode() : GpuProgramPassNode(GpuNodeType::se_GpuNodeType_ComputePass) {}
        ~ComputePassNode() = default;

        void Setup(IGpuNode** inputs, IGpuNode** outputs) override
        {
            // TODO @DavidMohrhardt: Implement
        }

        void Execute() override
        {
            // TODO @DavidMohrhardt: Implement
        }

    private:
    };

    class TransferPassNode : IGpuNode
    {
        TransferPassNode() : IGpuNode(GpuNodeType::se_GpuNodeType_Transfer) {}

        void Setup(IGpuNode** inputs, IGpuNode** outputs) override
        {
            // TODO @DavidMohrhardt: Implement
        }

        void Execute() override
        {
            GfxDriver::GetDriver()->EnqueueTransfer(m_pSource, m_pDestination);
        }

    private:
        GpuResource* m_pSource;
        GpuResource* m_pDestination;
    };

    /**
     * @brief A render graph node that represents the execution of a render pass.
     * It will wrap IGpuNodes and allow the RenderGraph to generate a dependency graph
     *
     */
    class RenderGraphNode
    {
    private:
        IGpuNode* m_pNode;
        dynamic_array<RenderGraphNode*> m_Children;

    public:
        RenderGraphNode(IGpuNode* pNode) : m_pNode(pNode), m_Children() {}
        ~RenderGraphNode() = default;

        void Setup(RenderGraphNode** inputs, RenderGraphNode** outputs)
        {
            m_pNode->Setup(inputs, outputs);
        }

        void Execute()
        {
            m_pNode->Execute();
        }
    };

    class RenderGraph
    {
    public:
        RenderGraph();
        ~RenderGraph();

        void Begin();
        void Finalize();

        void AddRenderNode(IGpuNode* pNode);
        void RemoveRenderNode(IGpuNode* pNode);

    private:
        void GenerateDependencyGraph();
        void ExecuteRenderNodes();

        dynamic_array<RenderGraphNode> m_GraphNodes;
        dynamic_array<dynamic_array<RenderGraphNode>> m_RenderTiers;
    };
} // namespace Savanna::Gfx
