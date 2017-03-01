#pragma once

class RenderData
{
public:
	RenderData();
	~RenderData();

	// Copy Constructor
	RenderData(const RenderData& other) = delete;
	// Move Constructor
	RenderData(RenderData&& other);

	// Operator
	RenderData& operator=(const RenderData& other) = delete;

	void GenerateBuffers();

	void Bind() const;
	void Unbind() const;
	void Render() const;


	unsigned int& GetVAO() { return m_VAO; }
	unsigned int& GetVBO() { return m_VBO; }
	unsigned int& GetIBO() { return m_IBO; }

	unsigned int GetVAO() const { return m_VAO; }
	unsigned int GetVBO() const { return m_VBO; }
	unsigned int GetIBO() const { return m_IBO; }

	void SetNumberofIndices(unsigned int numIndicies)
		{
			m_numbersofIndices = numIndicies;
	};
	unsigned int GetNumberOfIndicies() const { return m_numbersofIndices; };

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_numbersofIndices;
};