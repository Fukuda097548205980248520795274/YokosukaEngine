
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
    output.position = input.position;
    return output;
}