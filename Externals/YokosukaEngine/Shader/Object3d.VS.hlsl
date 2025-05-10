
// ���W�ϊ��̍s��
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// ���_�V�F�[�_�̏o��
struct VertexShaderOutput
{
    // �����N���b�v���
    float4 position : SV_POSITION;
};

// ���͂��ꂽ�l
struct VertexShaderInput
{
    // ���[�J�����W
    float4 position : POSITION0;
};

// main�֐�
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // �����N���b�v��Ԃɕϊ�����
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    return output;
}